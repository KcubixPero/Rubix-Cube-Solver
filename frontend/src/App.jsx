import { Canvas } from "@react-three/fiber";
import { OrbitControls } from "@react-three/drei";
import { useCallback, useEffect, useMemo, useRef, useState } from "react";
import Cube from "./Components/Cube/Cube";
import { COLORS } from "./Components/Cube/constants";
import { applyMove, createSolvedCube, tokenizeMoves } from "./Components/Cube/cubeState";
import "./App.css";

const MOVES = ["R", "R'", "R2", "L", "L'", "L2", "U", "U'", "U2", "D", "D'", "D2", "F", "F'", "F2", "B", "B'", "B2"];
const FACES = ["R", "U", "L", "B", "D", "F"];
const VIEWS = {
  R: [7, 2.8, 0], U: [0, 7, 0], L: [-7, 2.8, 0],
  B: [0, 2.8, -7], D: [0, -7, 0], F: [0, 2.8, 7],
};
const STAGES = [
  ["cross", "White cross"], ["f2l", "F2L"], ["oll", "OLL"], ["pll", "PLL"],
];

function moveCount(value = "") {
  return value.trim() ? value.trim().split(/\s+/).length : 0;
}

function makeScramble(length = 22) {
  const result = [];
  let previousFace = "";
  while (result.length < length) {
    const move = MOVES[Math.floor(Math.random() * MOVES.length)];
    if (move[0] === previousFace) continue;
    result.push(move);
    previousFace = move[0];
  }
  return result.join(" ");
}

function StageCard({ label, value, active, complete }) {
  return (
    <article className={`stage-card${active ? " is-active" : ""}${complete ? " is-complete" : ""}`}>
      <div className="stage-heading"><span>{label}</span><b>{moveCount(value)} moves</b></div>
      <p className="algorithm">{value || <span className="muted">Waiting for solve</span>}</p>
    </article>
  );
}

export default function App() {
  const [cube, setCube] = useState(createSolvedCube);
  const [scramble, setScramble] = useState("");
  const [customScramble, setCustomScramble] = useState("");
  const [solution, setSolution] = useState(null);
  const [status, setStatus] = useState("ready");
  const [error, setError] = useState("");
  const [view, setView] = useState("F");
  const [animation, setAnimation] = useState(null);
  const [paused, setPaused] = useState(false);
  const [theme, setTheme] = useState(() => localStorage.getItem("kcubixlab-theme") || (matchMedia("(prefers-color-scheme: dark)").matches ? "dark" : "light"));
  const controls = useRef(null);
  const queue = useRef([]);
  const running = useRef(false);
  const pausedRef = useRef(false);
  const completion = useRef(null);
  const moveId = useRef(0);
  const stages = useMemo(() => solution ? STAGES.map(([key, label]) => ({ key, label, value: solution[key] || "" })) : [], [solution]);

  const finishAnimation = useCallback((id) => { if (completion.current?.id === id) completion.current.resolve(); }, []);
  const startQueue = useCallback(async () => {
    if (running.current) return;
    running.current = true;
    while (queue.current.length) {
      while (pausedRef.current) await new Promise((resolve) => window.setTimeout(resolve, 100));
      const { token, stage } = queue.current.shift();
      if (stage) setStatus(stage);
      const id = ++moveId.current;
      const done = new Promise((resolve) => { completion.current = { id, resolve }; });
      setAnimation({ token, id });
      await done;
      setCube((current) => applyMove(current, token));
      setAnimation(null);
      completion.current = null;
    }
    running.current = false;
    setStatus("solved");
  }, []);
  const enqueueMoves = (moves, stage) => { queue.current.push(...moves.map((token) => ({ token, stage }))); startQueue(); };

  const clearSolution = () => {
    setSolution(null);
    setError("");
    setStatus("ready");
  };

  const setPuzzle = (sequence) => {
    const parsed = tokenizeMoves(sequence);
    const text = parsed.join(" ");
    queue.current = [];
    setCube(createSolvedCube());
    setScramble(text);
    setCustomScramble(text);
    clearSolution();
    enqueueMoves(parsed);
  };

  const onGenerate = () => {
    try { setPuzzle(makeScramble()); }
    catch (e) { setError(e.message); }
  };

  const onApply = () => {
    try {
      if (!customScramble.trim()) throw new Error("Enter a scramble first.");
      setPuzzle(customScramble);
    } catch (e) { setError(e.message); }
  };

  const onSolve = async () => {
    if (!scramble) return;
    setStatus("solving");
    setError("");
    try {
      const response = await fetch("/api/solve", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ scramble }),
      });
      const payload = await response.json();
      if (!response.ok) throw new Error(payload.error || "The solver could not solve this cube.");
      const next = Object.fromEntries(STAGES.map(([key]) => [key, tokenizeMoves(payload[key] || "").join(" ")]));
      const full = tokenizeMoves(payload.full || "").join(" ");
      const concatenated = STAGES.map(([key]) => next[key]).filter(Boolean).join(" ");
      if (full !== concatenated) throw new Error("Backend returned inconsistent stage and full solutions.");
      setSolution({ ...next, full });

      queue.current = [];
      setCube(createSolvedCube());
      enqueueMoves(tokenizeMoves(scramble));
      await new Promise((resolve) => { const timer = window.setInterval(() => { if (!running.current && !queue.current.length) { window.clearInterval(timer); resolve(); } }, 50); });
      for (const [key] of STAGES) enqueueMoves(tokenizeMoves(next[key]), key);
    } catch (e) {
      setStatus("error");
      setError(e.message || "Backend connection failed. Start the C++ solver service and retry.");
    }
  };

  const toggleTheme = () => setTheme((value) => value === "dark" ? "light" : "dark");
  useEffect(() => { document.documentElement.dataset.theme = theme; localStorage.setItem("kcubixlab-theme", theme); document.title = "KCubixLab — Rubik's Cube Solver"; }, [theme]);

  const onView = (face) => {
    setView(face);
    const camera = controls.current?.object;
    if (camera) {
      const [x, y, z] = VIEWS[face];
      camera.position.set(x, y, z);
      camera.lookAt(0, 0, 0);
      controls.current.update();
    }
  };

  const resetCube = () => {
    queue.current = [];
    completion.current?.resolve();
    completion.current = null;
    pausedRef.current = false;
    setPaused(false);
    setAnimation(null);
    setCube(createSolvedCube());
    setScramble("");
    setCustomScramble("");
    setSolution(null);
    setError("");
    setStatus("ready");
  };

  const fullSolution = solution?.full || "";
  const isBusy = status === "solving" || STAGES.some(([key]) => status === key);
  const copySolution = async () => {
    try { await navigator.clipboard.writeText(fullSolution); setError(""); }
    catch { setError("Clipboard access was blocked by the browser."); }
  };

  return (
    <main className="app-shell">
      <header className="topbar">
        <a className="brand" href="#top" aria-label="KCubixLab home"><span className="brand-mark">K</span><span>KCubix<span className="brand-accent">Lab</span></span></a>
        <div className="topbar-meta"><span className="status-dot" /> CFOP WORKSPACE <span className="topbar-divider">/</span> 3×3×3</div>
        <button className="theme-toggle" onClick={toggleTheme}>{theme === "dark" ? "Light mode" : "Dark mode"}</button><span className="version-tag">BETA</span>
      </header>

      <section className="intro" id="top">
        <div><p className="eyebrow">CUBE SOLVER <span>•</span> VISUALIZER</p><h1>Solve with clarity.</h1><p className="intro-copy">Scramble your cube, inspect every face, and follow each stage of CFOP.</p></div>
        <div className="method-pills">{["CROSS", "F2L", "OLL", "PLL"].map((step, i) => <span key={step}><b>0{i + 1}</b>{step}</span>)}</div>
      </section>

      <section className="workspace">
        <div className="cube-panel panel">
          <div className="panel-top"><div><p className="eyebrow">LIVE PUZZLE</p><h2>Cube view</h2></div><span className="view-label">{view} FACE VIEW</span></div>
          <div className="canvas-wrap">
            <Canvas camera={{ position: [6, 4.5, 7], fov: 34 }} dpr={[1, 1.6]}>
              <color attach="background" args={[theme === "dark" ? "#182329" : "#f5f7fa"]} />
              <ambientLight intensity={1.5} />
              <directionalLight position={[5, 8, 7]} intensity={2.4} />
              <directionalLight position={[-5, 2, -4]} intensity={1.1} />
              <Cube cubies={cube} animation={animation} paused={paused} onAnimationComplete={finishAnimation} />
              <OrbitControls ref={controls} enablePan={false} minDistance={5} maxDistance={11} enableDamping dampingFactor={0.08} />
            </Canvas>
            <span className="drag-hint"><span>↗</span> DRAG TO ROTATE</span>
          </div>
          <div className="view-controls"><span>ORIENT VIEW</span><div>{FACES.map((face) => <button key={face} className={view === face ? "selected" : ""} onClick={() => onView(face)} disabled={isBusy} aria-label={`View ${face} face`}>{face}</button>)}</div><button className="reset-view" onClick={() => onView("F")} disabled={isBusy}>Front view</button></div>
          <div className="playback-controls"><button onClick={() => { pausedRef.current = !pausedRef.current; setPaused(pausedRef.current); }}>{paused ? "Resume" : "Pause"}</button><button className="cube-reset-button" onClick={resetCube}>Reset cube</button></div>
          <div className="color-key">{Object.entries({ W: COLORS.WHITE, R: COLORS.RED, B: COLORS.BLUE, G: COLORS.GREEN, O: COLORS.ORANGE, Y: COLORS.YELLOW }).map(([label, color]) => <span key={label}><i style={{ background: color }} />{label}</span>)}</div>
        </div>

        <aside className="control-column">
          <section className="panel scramble-panel">
            <div className="panel-top compact"><div><p className="eyebrow">01 / INPUT</p><h2>Scramble</h2></div><span className="cube-state">{scramble ? "SCRAMBLED" : "SOLVED"}</span></div>
            <button className="primary-button" onClick={onGenerate} disabled={isBusy}><span className="shuffle-icon">⤨</span> Generate scramble</button>
            <div className="scramble-display">{scramble || <span className="muted">Your scramble will appear here</span>}</div>
            <div className="divider-label"><span>OR ENTER YOUR OWN</span></div>
            <div className="custom-row"><input aria-label="Custom scramble" placeholder="R U R' U' …" value={customScramble} onChange={(e) => setCustomScramble(e.target.value)} disabled={isBusy} onKeyDown={(e) => e.key === "Enter" && onApply()} /><button className="secondary-button" onClick={onApply} disabled={isBusy}>Apply</button></div>
            <p className="input-hint">Standard face turns: R L U D F B · add ' or 2</p>
          </section>

          <section className="solve-card">
            <div><p className="eyebrow">02 / SOLUTION</p><h2>{status === "solving" || STAGES.some(([key]) => key === status) ? "Working through the cube" : status === "solved" ? "Cube solved" : "Ready when you are"}</h2><p>{scramble ? `${moveCount(scramble)} scramble moves loaded` : "Generate or enter a scramble to begin"}</p></div>
            <button className="solve-button" onClick={onSolve} disabled={!scramble || isBusy}>{isBusy ? <><span className="spinner" />Solving</> : "Solve cube"}<span className="solve-arrow">↗</span></button>
          </section>

          {error && <div className="error-banner" role="alert"><b>Couldn’t continue</b><span>{error}</span></div>}

          {solution && <section className="solution-panel panel">
            <div className="panel-top compact"><div><p className="eyebrow">03 / BREAKDOWN</p><h2>Solution</h2></div><span className={`result-badge ${status === "solved" ? "done" : ""}`}>{status === "solved" ? "SOLVED" : "PLAYING"}</span></div>
            <div className="stages-list">{stages.map(({ key, label, value }) => <StageCard key={key} label={label} value={value} active={status === key} complete={status === "solved" || STAGES.findIndex(([name]) => name === status) > STAGES.findIndex(([name]) => name === key)} />)}</div>
            <div className="complete-solution"><div className="stage-heading"><span>COMPLETE SOLUTION</span><b>{moveCount(fullSolution)} moves</b></div><p className="algorithm">{fullSolution || <span className="muted">No moves needed</span>}</p><button className="copy-button" onClick={copySolution}>Copy solution <span>⧉</span></button></div>
          </section>}
        </aside>
      </section>
      <footer><span>KCubixLab <span className="footer-dot">•</span> 3D cube uses the project’s W / R / B / G / O / Y orientation</span><span>MADE FOR THE NEXT MOVE</span></footer>
    </main>
  );
}
