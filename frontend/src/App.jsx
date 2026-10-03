import { Canvas } from "@react-three/fiber";
import { OrbitControls } from "@react-three/drei";
import { useCallback, useEffect, useMemo, useRef, useState } from "react";
import Cube from "./Components/Cube/Cube";
import { COLORS } from "./Components/Cube/constants";
import { applyMove, createSolvedCube, serializeCubeState, tokenizeMoves } from "./Components/Cube/cubeState";
import "./App.css";

const MOVE_BUTTONS = ["R", "R'", "R2", "L", "L'", "L2", "U", "U'", "U2", "D", "D'", "D2", "F", "F'", "F2", "B", "B'", "B2"];
const STAGES = [
  { key: "cross", label: "WHITE CROSS" },
  { key: "f2l", label: "F2L" },
  { key: "oll", label: "OLL" },
  { key: "pll", label: "PLL" },
];
const API_BASE = import.meta.env.VITE_API_URL || "";

function makeScramble(length = 22) {
  const moves = [];
  let lastFace = "";
  while (moves.length < length) {
    const token = MOVE_BUTTONS[Math.floor(Math.random() * MOVE_BUTTONS.length)];
    if (token[0] === lastFace) continue;
    moves.push(token);
    lastFace = token[0];
  }
  return moves;
}

function inverseMove(token) {
  if (token.endsWith("2")) return token;
  return token.endsWith("'") ? token[0] : `${token}'`;
}

function cubeKey(cubies) {
  return JSON.stringify(cubies.map(({ id, position, stickers }) => [id, position, Object.entries(stickers).sort()]));
}

function formatMoves(moves, currentIndex) {
  if (!moves.length) return <span className="muted">No moves needed</span>;
  return moves.map((move, index) => <span className={index === currentIndex ? "move-token is-next" : "move-token"} key={`${index}-${move}`}>{move}{index < moves.length - 1 ? " " : ""}</span>);
}

export default function App() {
  const [cube, setCube] = useState(createSolvedCube);
  const [scramble, setScramble] = useState("");
  const [customScramble, setCustomScramble] = useState("");
  const [solution, setSolution] = useState(null);
  const [stageIndex, setStageIndex] = useState(0);
  const [moveIndex, setMoveIndex] = useState(0);
  const [status, setStatus] = useState("ready");
  const [error, setError] = useState("");
  const [animation, setAnimation] = useState(null);
  const [paused, setPaused] = useState(false);
  const [animating, setAnimating] = useState(false);
  const [playbackActive, setPlaybackActive] = useState(false);
  const [theme, setTheme] = useState(() => {
    try { return localStorage.getItem("kcubixlab-theme") || (matchMedia("(prefers-color-scheme: dark)").matches ? "dark" : "light"); }
    catch { return "light"; }
  });

  const queue = useRef([]);
  const running = useRef(false);
  const pausedRef = useRef(false);
  const completion = useRef(null);
  const moveId = useRef(0);
  const queueVersion = useRef(0);
  const stageRef = useRef(0);
  const moveIndexRef = useRef(0);
  const requestRef = useRef(null);

  useEffect(() => {
    document.documentElement.dataset.theme = theme;
    try { localStorage.setItem("kcubixlab-theme", theme); } catch { /* Storage may be unavailable in private mode. */ }
    document.title = "KCUBIXLAB - Rubik's Cube Solver";
  }, [theme]);

  const setPlaybackPosition = (nextStage, nextMove) => {
    stageRef.current = nextStage;
    moveIndexRef.current = nextMove;
    setStageIndex(nextStage);
    setMoveIndex(nextMove);
  };

  const finishAnimation = useCallback((id) => {
    if (completion.current?.id === id) completion.current.resolve();
  }, []);

  const startQueue = useCallback(async () => {
    if (running.current) return;
    running.current = true;
    const version = queueVersion.current;
    setAnimating(true);
    try {
      while (queue.current.length && version === queueVersion.current) {
        while (pausedRef.current && version === queueVersion.current)
          await new Promise((resolve) => window.setTimeout(resolve, 40));
        if (version !== queueVersion.current) break;
        const item = queue.current.shift();
        const id = ++moveId.current;
        const finished = new Promise((resolve) => { completion.current = { id, resolve }; });
        setAnimation({ token: item.token, id });
        await finished;
        if (version !== queueVersion.current) break;
        setCube((current) => applyMove(current, item.token));
        setAnimation(null);
        completion.current = null;
        item.onCommit?.();
      }
    } finally {
      running.current = false;
      setAnimating(false);
      if (queue.current.length && !pausedRef.current) startQueue();
    }
  }, []);

  const enqueue = (items) => {
    queue.current.push(...items);
    if (!running.current) startQueue();
  };

  const cancelQueue = () => {
    queueVersion.current += 1;
    queue.current = [];
    completion.current?.resolve();
    completion.current = null;
    setAnimation(null);
    setAnimating(false);
    pausedRef.current = false;
    setPaused(false);
  };

  const clearSolution = () => {
    setSolution(null);
    setPlaybackPosition(0, 0);
    setPlaybackActive(false);
    setStatus("ready");
  };

  const cancelRequest = () => {
    if (requestRef.current) {
      requestRef.current.canceled = true;
      window.clearTimeout(requestRef.current.timer);
      requestRef.current.controller.abort();
      requestRef.current = null;
    }
  };

  const applyScramble = (moves) => {
    cancelRequest();
    cancelQueue();
    clearSolution();
    setError("");
    setScramble(moves.join(" "));
    setCustomScramble(moves.join(" "));
    setCube(createSolvedCube());
    enqueue(moves.map((token) => ({ token })));
  };

  const onGenerateScramble = () => {
    try { applyScramble(makeScramble()); }
    catch (cause) { setError(cause.message); }
  };

  const onApplyScramble = () => {
    try {
      if (!customScramble.trim()) throw new Error("Enter a scramble first.");
      applyScramble(tokenizeMoves(customScramble));
    } catch (cause) { setError(cause.message); }
  };

  const onSolve = async () => {
    cancelRequest();
    setSolution(null);
    setPlaybackPosition(0, 0);
    setStatus("solving");
    setError("");
    const controller = new AbortController();
    const timer = window.setTimeout(() => controller.abort(), 60000);
    requestRef.current = { controller, timer, canceled: false };
    const activeRequest = requestRef.current;
    try {
      const cubeState = serializeCubeState(cube);
      const response = await fetch(`${API_BASE}/api/solve`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ cube: cubeState }),
        signal: controller.signal,
      });
      const responseText = await response.text();
      let payload;
      try {
        payload = responseText ? JSON.parse(responseText) : null;
      } catch {
        throw new Error(`Solver returned an invalid response (${response.status}). Check the backend console and retry.`);
      }
      if (!payload) {
        throw new Error(`Solver returned an empty response (${response.status}). Check that the backend is running and retry.`);
      }
      if (!response.ok) throw new Error(payload.error || `Solver request failed (${response.status}).`);
      const nextSolution = Object.fromEntries(STAGES.map(({ key }) => {
        if (!Array.isArray(payload[key]) || payload[key].some((move) => typeof move !== "string"))
          throw new Error(`Solver response is missing a valid ${key.toUpperCase()} move list.`);
        return [key, tokenizeMoves(payload[key].join(" "))];
      }));
      if (payload.counts) {
        for (const { key } of STAGES)
          if (payload.counts[key] !== nextSolution[key].length) throw new Error(`Solver response count mismatch for ${key.toUpperCase()}.`);
      }
      const resultCube = STAGES.reduce((state, { key }) => nextSolution[key].reduce(applyMove, state), cube);
      if (cubeKey(resultCube) !== cubeKey(createSolvedCube())) throw new Error("Solver response did not produce a solved cube.");
      setSolution(nextSolution);
      setPlaybackPosition(0, 0);
      setStatus("solution-ready");
    } catch (cause) {
      if (activeRequest.canceled) return;
      if (cause.name === "AbortError") setError("The solver timed out or the request was canceled. Retry when the backend is available.");
      else setError(cause.message || "Could not connect to the C++ solver. Start the backend API and retry.");
      setStatus("error");
    } finally {
      if (requestRef.current?.controller === controller) {
        window.clearTimeout(timer);
        requestRef.current = null;
      }
    }
  };

  const currentMoves = solution?.[STAGES[stageIndex]?.key] || [];
  const advanceStage = () => {
    const nextStage = stageRef.current + 1;
    if (nextStage <= STAGES.length) setPlaybackPosition(nextStage, 0);
    if (nextStage >= STAGES.length) setStatus("solved");
  };

  const nextMove = () => {
    if (!solution || animating || paused) return;
    const currentStage = stageRef.current;
    if (currentStage >= STAGES.length) return;
    const moves = solution[STAGES[currentStage].key];
    const index = moveIndexRef.current;
    if (index >= moves.length) { advanceStage(); return; }
    enqueue([{ token: moves[index], onCommit: () => {
      if (stageRef.current === currentStage) setPlaybackPosition(currentStage, index + 1);
    } }]);
  };

  const previousMove = () => {
    if (!solution || animating || paused) return;
    let currentStage = stageRef.current;
    let index = moveIndexRef.current;
    if (currentStage >= STAGES.length) {
      currentStage = STAGES.length - 1;
      index = solution[STAGES[currentStage].key].length;
      setPlaybackPosition(currentStage, index);
    }
    if (index === 0 && currentStage > 0) {
      currentStage -= 1;
      setPlaybackPosition(currentStage, solution[STAGES[currentStage].key].length);
      index = moveIndexRef.current;
    }
    if (index === 0) return;
    const token = solution[STAGES[currentStage].key][index - 1];
    enqueue([{ token: inverseMove(token), onCommit: () => setPlaybackPosition(currentStage, index - 1) }]);
  };

  const playPause = () => {
    if (pausedRef.current) {
      pausedRef.current = false;
      setPaused(false);
      setPlaybackActive(true);
      if (!running.current && queue.current.length) startQueue();
      return;
    }
    if (running.current) {
      pausedRef.current = true;
      setPaused(true);
      return;
    }
    if (!solution || stageRef.current >= STAGES.length) return;
    const currentStage = stageRef.current;
    const moves = solution[STAGES[currentStage].key];
    const from = moveIndexRef.current;
    if (from >= moves.length) { advanceStage(); return; }
    setPlaybackActive(true);
    enqueue(moves.slice(from).map((token, offset) => ({ token, onCommit: () => {
      if (stageRef.current === currentStage) setPlaybackPosition(currentStage, from + offset + 1);
      if (offset === moves.length - from - 1) setPlaybackActive(false);
    } })));
  };

  const onManualMove = (token) => {
    if (animating || status === "solving") return;
    cancelQueue();
    cancelRequest();
    clearSolution();
    setScramble("");
    setError("");
    enqueue([{ token }]);
  };

  const resetCube = () => {
    cancelRequest();
    cancelQueue();
    clearSolution();
    setCube(createSolvedCube());
    setScramble("");
    setCustomScramble("");
    setError("");
  };

  const activeStage = STAGES[stageIndex];
  const isSolved = stageIndex >= STAGES.length;
  const totalMoves = useMemo(() => solution ? STAGES.reduce((sum, { key }) => sum + solution[key].length, 0) : 0, [solution]);
  const isLocked = animating || status === "solving";
  const toggleTheme = () => setTheme((current) => current === "dark" ? "light" : "dark");
  const copySolution = async () => {
    const allMoves = STAGES.flatMap(({ key }) => solution?.[key] || []).join(" ");
    try { await navigator.clipboard.writeText(allMoves); setError(""); }
    catch { setError("Clipboard access was blocked by the browser."); }
  };

  return (
    <main className="app-shell">
      <header className="topbar">
        <a className="brand" href="#top" aria-label="KCUBIXLAB home"><span className="brand-mark">K</span><span>KCUBIXLAB</span></a>
        <div className="topbar-meta"><span className="status-dot" /> CFOP WORKSPACE <span className="topbar-divider">/</span> 3x3x3</div>
        <button className="theme-toggle" onClick={toggleTheme}>{theme === "dark" ? "Light mode" : "Dark mode"}</button>
        <span className="version-tag">BETA</span>
      </header>

      <section className="intro" id="top">
        <div><p className="eyebrow">CUBE SOLVER | VISUALIZER</p><h1>Solve with clarity.</h1><p className="intro-copy">Scramble your cube, inspect the state, and follow each solving stage move by move.</p></div>
        <div className="method-pills">{STAGES.map(({ label }, index) => <span key={label}><b>0{index + 1}</b>{label}</span>)}</div>
      </section>

      <section className="workspace">
        <div className="cube-panel panel">
          <div className="panel-top"><div><p className="eyebrow">LIVE PUZZLE</p><h2>Cube view</h2></div><span className="view-label">DRAG TO ROTATE</span></div>
          <div className="canvas-wrap">
            <Canvas camera={{ position: [6, 4.5, 7], fov: 34 }} dpr={[1, 1.6]}>
              <color attach="background" args={[theme === "dark" ? "#182329" : "#f5f7fa"]} />
              <ambientLight intensity={1.5} />
              <directionalLight position={[5, 8, 7]} intensity={2.4} />
              <directionalLight position={[-5, 2, -4]} intensity={1.1} />
              <Cube cubies={cube} animation={animation} paused={paused} onAnimationComplete={finishAnimation} />
              <OrbitControls enablePan={false} minDistance={5} maxDistance={11} enableDamping dampingFactor={0.08} />
            </Canvas>
          </div>
          <div className="color-key">{Object.entries({ W: COLORS.WHITE, R: COLORS.RED, B: COLORS.BLUE, G: COLORS.GREEN, O: COLORS.ORANGE, Y: COLORS.YELLOW }).map(([label, color]) => <span key={label}><i style={{ background: color }} />{label}</span>)}</div>
          <div className="cube-actions"><button className="secondary-button" onClick={resetCube}>Reset cube</button></div>
        </div>

        <aside className="control-column">
          <section className="panel scramble-panel">
            <div className="panel-top compact"><div><p className="eyebrow">01 / INPUT</p><h2>Scramble</h2></div><span className="cube-state">{scramble ? "LOADED" : "CURRENT STATE"}</span></div>
            <button className="primary-button" onClick={onGenerateScramble} disabled={isLocked}>Generate scramble</button>
            <div className="scramble-display">{scramble || <span className="muted">Enter a scramble or use the manual controls below</span>}</div>
            <div className="divider-label"><span>OR ENTER YOUR OWN</span></div>
            <div className="custom-row"><input aria-label="Custom scramble" placeholder="R U R' U' ..." value={customScramble} onChange={(event) => setCustomScramble(event.target.value)} disabled={isLocked} onKeyDown={(event) => event.key === "Enter" && onApplyScramble()} /><button className="secondary-button" onClick={onApplyScramble} disabled={isLocked}>Apply</button></div>
            <p className="input-hint">Standard face turns: R L U D F B; add ' or 2</p>
          </section>

          <section className="solve-card">
            <div><p className="eyebrow">02 / SOLUTION</p><h2>{status === "solving" ? "Contacting solver" : isSolved ? "Cube solved" : solution ? "Stage playback ready" : "Ready when you are"}</h2><p>{solution ? `${totalMoves} moves across four stages` : "Solve the current cube state with the C++ backend"}</p></div>
            <button className="solve-button" onClick={onSolve} disabled={isLocked}>{status === "solving" ? <><span className="spinner" />Solving</> : "Solve cube"}</button>
          </section>

          {error && <div className="error-banner" role="alert"><b>Could not continue</b><span>{error}</span></div>}

          {solution && <section className="solution-panel panel">
            <div className="panel-top compact"><div><p className="eyebrow">03 / STAGE PLAYBACK</p><h2>{isSolved ? "SOLVED" : activeStage.label}</h2></div><span className={`result-badge ${isSolved ? "done" : ""}`}>{isSolved ? "COMPLETE" : `STAGE ${stageIndex + 1} / 4`}</span></div>
            <div className="playback-summary">
              {isSolved ? <b>All stages complete</b> : <><b>Move {moveIndex} / {currentMoves.length}</b><span>{currentMoves.length} moves in this stage</span></>}
            </div>
            {!isSolved && <p className="algorithm current-algorithm">{formatMoves(currentMoves, moveIndex)}</p>}
            <div className="playback-controls">
              <button onClick={previousMove} disabled={isLocked || !solution || (stageIndex === 0 && moveIndex === 0)} aria-label="Previous move">Previous move</button>
              <button className="play-button" onClick={playPause} disabled={!solution || isSolved || (animating && !playbackActive)}>{paused ? "Resume" : playbackActive ? "Pause" : "Play"}</button>
              <button className="next-button" onClick={nextMove} disabled={isLocked || isSolved}>{moveIndex >= currentMoves.length ? (stageIndex === STAGES.length - 1 ? "Finish" : "Next stage") : "Next move"}</button>
            </div>
            <div className="stage-counts">{STAGES.map(({ key, label }, index) => <div className={index === stageIndex ? "stage-count is-current" : index < stageIndex ? "stage-count is-done" : "stage-count"} key={key}><span>{label}</span><b>{solution[key].length}</b></div>)}</div>
            <button className="copy-button" onClick={copySolution}>Copy all moves</button>
          </section>}
        </aside>
      </section>

      <section className="manual-panel panel">
        <div className="panel-top compact"><div><p className="eyebrow">04 / MANUAL CONTROL</p><h2>Manual moves</h2></div><span className="view-label">18 FACE TURNS</span></div>
        <div className="manual-grid">{["R", "L", "U", "D", "F", "B"].map((face) => <div className="manual-move-row" key={face}>{MOVE_BUTTONS.filter((move) => move[0] === face).map((move) => <button key={move} onClick={() => onManualMove(move)} disabled={isLocked} aria-label={`Perform ${move}`}>{move}</button>)}</div>)}</div>
      </section>

      <footer><span>KCUBIXLAB | 3D cube uses W / R / B / G / O / Y face colors</span><span>MADE FOR THE NEXT MOVE</span></footer>
    </main>
  );
}
