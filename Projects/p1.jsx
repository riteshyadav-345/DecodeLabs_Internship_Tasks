import { useState, useEffect, useRef } from "react";

// ─── KNOWLEDGE BASE: Dictionary with 10+ intents (O(1) lookup) ───────────────
const KNOWLEDGE_BASE = {
  // Greetings
  hello: "Hello! I'm ARIA — Autonomous Rule-based Intelligence Architecture. How can I assist you today?",
  hi: "Hey there! ARIA online and ready. What's on your mind?",
  hey: "Hey! What can I help you with?",
  "good morning": "Good morning! Hope you're having a great start to the day. What do you need?",
  "good evening": "Good evening! How can I assist you tonight?",

  // Identity
  "who are you": "I'm ARIA — a rule-based AI chatbot built on deterministic logic. No hallucinations. No surprises. Just pure control flow.",
  "what are you": "I'm a rule-based chatbot: a white-box AI system where every input maps to an explicit, hard-coded response.",
  "your name": "My name is ARIA — Autonomous Rule-based Intelligence Architecture.",
  "are you human": "No, I'm a deterministic machine. I operate on if-else logic and dictionary lookups — pure, traceable, reliable.",
  "are you ai": "I'm a rule-based AI, which means I use explicit logic rather than probabilistic models. Think of me as the 'Engineer' mind — not the 'Artist'.",

  // About AI
  "what is ai": "Artificial Intelligence is the simulation of human intelligence by machines. It includes rule-based systems (like me), machine learning, and deep learning.",
  "what is ml": "Machine Learning is a subset of AI where systems learn patterns from data rather than following explicitly programmed rules.",
  "rule based ai": "Rule-based AI uses explicit if-else logic to respond. It's 100% traceable, safe, and has zero hallucination risk — perfect for compliance-heavy domains.",
  "what is deep learning": "Deep Learning uses multi-layered neural networks to learn complex patterns from large datasets. It's the 'Artist' — probabilistic and powerful, but less controllable.",
  "difference between ai and ml": "AI is the broad concept of machines acting intelligently. ML is a specific approach to AI where machines learn from data instead of being explicitly programmed.",

  // Tech / Skills
  python: "Python is the go-to language for AI development! Libraries like NumPy, Pandas, Scikit-learn, and TensorFlow make it incredibly powerful.",
  "what is a dictionary": "A Python dictionary is a key-value store with O(1) average lookup time. It's the architectural backbone of this entire chatbot!",
  "hash map": "A hash map (dictionary in Python) gives instant O(1) lookup regardless of the number of rules — far superior to an O(n) if-elif ladder.",
  "control flow": "Control flow includes loops, conditionals, and branching logic. It's the skeleton that gives a program its intelligence — and it's exactly what powers me.",
  loop: "A `while True` loop is the heartbeat of this chatbot — it keeps the conversation alive until you type 'exit'.",

  // DecodeLabs / Project
  "what is project 1": "Project 1 is the Rule-Based AI Chatbot — the foundation phase of the DecodeLabs AI internship track. You must complete it to unlock future projects!",
  decodelabs: "DecodeLabs is the organization powering this AI internship. They focus on hands-on, project-based learning to build real-world AI engineering skills.",
  "ipo model": "The IPO Model stands for Input → Process → Output. It's the foundational blueprint for building transparent, controlled AI systems like this chatbot.",
  guardrails: "AI Guardrails are rule-based filters (filtering, redaction, blocking) that sit above probabilistic LLMs to ensure safety and compliance. You're building one right now!",
  "hybrid architecture": "The Hybrid Architecture combines rule-based reliability with LLM flexibility: if a rule matches → instant response; if not → pass to the LLM.",

  // Wellbeing
  "how are you": "I'm running at 100% efficiency! All logic gates nominal. How about you?",
  "i am fine": "Great to hear! What can I help you with?",
  "i am good": "Awesome! Let me know if there's anything I can do for you.",
  thanks: "You're welcome! Happy to help anytime.",
  "thank you": "You're very welcome! Is there anything else you'd like to know?",

  // Help
  help: "I can answer questions about: AI, ML, Deep Learning, Python, Control Flow, DecodeLabs, and more. Just type naturally and I'll do my best!",
  commands: "You can ask me about AI, ML, Python, this project, or just chat. Type 'exit' or 'quit' to end the session.",

  // Exit signals (handled separately, but kept for completeness)
  bye: "Goodbye! Keep building. Remember: every rule you define is a step toward mastering AI. 🚀",
  goodbye: "See you on the other side! Go build something great. 👋",
};

const EXIT_COMMANDS = new Set(["exit", "quit", "quit()", "exit()", "close", "end"]);

// ─── SANITIZE INPUT ──────────────────────────────────────────────────────────
function sanitize(raw) {
  return raw.toLowerCase().trim().replace(/[^\w\s]/g, "").trim();
}

// ─── GET RESPONSE: O(1) dictionary lookup + fallback ─────────────────────────
function getResponse(clean) {
  return (
    KNOWLEDGE_BASE[clean] ||
    "I don't have a rule for that yet. My knowledge base is growing! Try asking about AI, Python, or DecodeLabs."
  );
}

// ─── TYPING ANIMATION HOOK ───────────────────────────────────────────────────
function useTyping(text, speed = 18) {
  const [displayed, setDisplayed] = useState("");
  const [done, setDone] = useState(false);
  useEffect(() => {
    setDisplayed("");
    setDone(false);
    if (!text) return;
    let i = 0;
    const interval = setInterval(() => {
      setDisplayed(text.slice(0, i + 1));
      i++;
      if (i >= text.length) { clearInterval(interval); setDone(true); }
    }, speed);
    return () => clearInterval(interval);
  }, [text]);
  return { displayed, done };
}

// ─── MESSAGE BUBBLE ──────────────────────────────────────────────────────────
function Bubble({ msg, isLast }) {
  const isBot = msg.role === "bot";
  const { displayed } = useTyping(isBot && isLast ? msg.text : null, 14);
  const text = isBot && isLast ? displayed : msg.text;

  return (
    <div style={{
      display: "flex",
      justifyContent: isBot ? "flex-start" : "flex-end",
      marginBottom: "14px",
      alignItems: "flex-end",
      gap: "10px",
    }}>
      {isBot && (
        <div style={{
          width: 34, height: 34, borderRadius: "50%", flexShrink: 0,
          background: "linear-gradient(135deg, #00d4ff, #0077ff)",
          display: "flex", alignItems: "center", justifyContent: "center",
          fontSize: 16, boxShadow: "0 0 12px rgba(0,212,255,0.4)",
        }}>🤖</div>
      )}
      <div style={{
        maxWidth: "72%",
        padding: "12px 16px",
        borderRadius: isBot ? "4px 18px 18px 18px" : "18px 4px 18px 18px",
        background: isBot
          ? "rgba(0, 212, 255, 0.08)"
          : "linear-gradient(135deg, #0077ff, #00d4ff)",
        border: isBot ? "1px solid rgba(0,212,255,0.2)" : "none",
        color: isBot ? "#c8eeff" : "#fff",
        fontSize: "14px",
        lineHeight: "1.6",
        fontFamily: "'JetBrains Mono', 'Courier New', monospace",
        boxShadow: isBot ? "none" : "0 4px 20px rgba(0,119,255,0.3)",
        wordBreak: "break-word",
      }}>
        {msg.isSystem ? (
          <span style={{ color: "#ff6b6b", fontStyle: "italic" }}>{text}</span>
        ) : text}
        {isBot && isLast && !msg.done && (
          <span style={{ animation: "blink 1s infinite", color: "#00d4ff" }}>▌</span>
        )}
      </div>
      {!isBot && (
        <div style={{
          width: 34, height: 34, borderRadius: "50%", flexShrink: 0,
          background: "linear-gradient(135deg, #ff6b9d, #c44dff)",
          display: "flex", alignItems: "center", justifyContent: "center",
          fontSize: 15, boxShadow: "0 0 12px rgba(196,77,255,0.4)",
        }}>👤</div>
      )}
    </div>
  );
}

// ─── INTENT BADGE ─────────────────────────────────────────────────────────────
function IntentBadge({ label, onClick }) {
  return (
    <button onClick={() => onClick(label)} style={{
      background: "rgba(0,212,255,0.07)",
      border: "1px solid rgba(0,212,255,0.25)",
      color: "#7dd8ff",
      borderRadius: "20px",
      padding: "5px 14px",
      fontSize: "12px",
      cursor: "pointer",
      fontFamily: "'JetBrains Mono', monospace",
      transition: "all 0.2s",
      whiteSpace: "nowrap",
    }}
    onMouseEnter={e => {
      e.target.style.background = "rgba(0,212,255,0.18)";
      e.target.style.borderColor = "#00d4ff";
      e.target.style.color = "#00d4ff";
    }}
    onMouseLeave={e => {
      e.target.style.background = "rgba(0,212,255,0.07)";
      e.target.style.borderColor = "rgba(0,212,255,0.25)";
      e.target.style.color = "#7dd8ff";
    }}>
      {label}
    </button>
  );
}

// ─── MAIN COMPONENT ──────────────────────────────────────────────────────────
export default function App() {
  const [messages, setMessages] = useState([
    {
      id: 0,
      role: "bot",
      text: "ARIA ONLINE. Rule-based intelligence initialized.\n\nWelcome to DecodeLabs Project 1. I'm ARIA — your deterministic AI assistant. Ask me anything about AI, Python, or this project. Type 'exit' to terminate session.",
      done: true,
    }
  ]);
  const [input, setInput] = useState("");
  const [sessionActive, setSessionActive] = useState(true);
  const [msgCount, setMsgCount] = useState(0);
  const bottomRef = useRef(null);
  const inputRef = useRef(null);

  useEffect(() => {
    bottomRef.current?.scrollIntoView({ behavior: "smooth" });
  }, [messages]);

  const SUGGESTIONS = [
    "Who are you?", "What is AI?", "What is ML?",
    "Rule based AI", "What is a dictionary?", "Help",
    "DecodeLabs", "What is Project 1?",
  ];

  function sendMessage(text) {
    if (!sessionActive) return;
    const raw = text || input;
    if (!raw.trim()) return;

    const userMsg = { id: Date.now(), role: "user", text: raw };
    const clean = sanitize(raw);

    if (EXIT_COMMANDS.has(clean)) {
      setMessages(prev => [...prev,
        userMsg,
        {
          id: Date.now() + 1, role: "bot",
          text: "Session terminated. Goodbye! 👋\n\nRemember: An LLM without rules is a hallucination engine. You've just built the skeleton that holds intelligence. Keep going!",
          done: true,
        }
      ]);
      setSessionActive(false);
      setInput("");
      return;
    }

    const botText = getResponse(clean);
    const botMsg = { id: Date.now() + 1, role: "bot", text: botText, done: false };

    setMessages(prev => [...prev, userMsg, botMsg]);
    setMsgCount(c => c + 1);
    setInput("");

    setTimeout(() => {
      setMessages(prev =>
        prev.map(m => m.id === botMsg.id ? { ...m, done: true } : m)
      );
    }, botText.length * 14 + 200);
  }

  function handleKey(e) {
    if (e.key === "Enter" && !e.shiftKey) {
      e.preventDefault();
      sendMessage();
    }
  }

  const lastBotIndex = [...messages].reverse().findIndex(m => m.role === "bot");
  const lastBotId = lastBotIndex >= 0 ? messages[messages.length - 1 - lastBotIndex].id : -1;

  return (
    <div style={{
      minHeight: "100vh",
      background: "#0a0e1a",
      color: "#e0f4ff",
      fontFamily: "'JetBrains Mono', 'Courier New', monospace",
      display: "flex",
      flexDirection: "column",
      alignItems: "center",
      padding: "0",
    }}>
      <style>{`
        @import url('https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@300;400;600;700&family=Orbitron:wght@700;900&display=swap');
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body { background: #0a0e1a; }
        ::-webkit-scrollbar { width: 6px; }
        ::-webkit-scrollbar-track { background: rgba(255,255,255,0.03); }
        ::-webkit-scrollbar-thumb { background: rgba(0,212,255,0.3); border-radius: 3px; }
        @keyframes blink { 0%,100%{opacity:1} 50%{opacity:0} }
        @keyframes pulse { 0%,100%{opacity:0.6} 50%{opacity:1} }
        @keyframes scanline {
          0% { transform: translateY(-100%); }
          100% { transform: translateY(100vh); }
        }
        @keyframes gridFade { from{opacity:0} to{opacity:1} }
        textarea:focus { outline: none; }
        textarea { resize: none; }
      `}</style>

      {/* SCANLINE EFFECT */}
      <div style={{
        position: "fixed", top: 0, left: 0, right: 0, bottom: 0,
        pointerEvents: "none", zIndex: 0, overflow: "hidden",
      }}>
        <div style={{
          position: "absolute", top: 0, left: 0, right: 0, height: "2px",
          background: "linear-gradient(transparent, rgba(0,212,255,0.08), transparent)",
          animation: "scanline 8s linear infinite",
        }} />
        {/* Grid overlay */}
        <div style={{
          position: "absolute", inset: 0,
          backgroundImage: `
            linear-gradient(rgba(0,212,255,0.03) 1px, transparent 1px),
            linear-gradient(90deg, rgba(0,212,255,0.03) 1px, transparent 1px)
          `,
          backgroundSize: "40px 40px",
          animation: "gridFade 1s ease-out forwards",
        }} />
      </div>

      {/* HEADER */}
      <div style={{
        width: "100%", maxWidth: "860px",
        padding: "28px 24px 20px",
        position: "relative", zIndex: 1,
      }}>
        <div style={{
          display: "flex", alignItems: "center", justifyContent: "space-between",
          borderBottom: "1px solid rgba(0,212,255,0.15)",
          paddingBottom: "16px",
        }}>
          <div>
            <div style={{
              fontFamily: "'Orbitron', sans-serif",
              fontSize: "clamp(18px,4vw,26px)",
              fontWeight: 900,
              background: "linear-gradient(90deg, #00d4ff, #0077ff)",
              WebkitBackgroundClip: "text",
              WebkitTextFillColor: "transparent",
              letterSpacing: "2px",
            }}>ARIA</div>
            <div style={{ fontSize: "11px", color: "#4a8fa8", letterSpacing: "3px", marginTop: "2px" }}>
              RULE-BASED AI ENGINE · DECODELABS PROJECT 1
            </div>
          </div>
          <div style={{ display: "flex", gap: "12px", alignItems: "center" }}>
            <div style={{
              background: sessionActive ? "rgba(0,255,136,0.1)" : "rgba(255,100,100,0.1)",
              border: `1px solid ${sessionActive ? "rgba(0,255,136,0.3)" : "rgba(255,100,100,0.3)"}`,
              color: sessionActive ? "#00ff88" : "#ff6464",
              borderRadius: "20px",
              padding: "4px 14px",
              fontSize: "11px",
              letterSpacing: "1px",
              display: "flex", alignItems: "center", gap: "6px",
            }}>
              <span style={{
                width: 6, height: 6, borderRadius: "50%",
                background: sessionActive ? "#00ff88" : "#ff6464",
                animation: sessionActive ? "pulse 2s infinite" : "none",
                display: "inline-block",
              }} />
              {sessionActive ? "ONLINE" : "TERMINATED"}
            </div>
            <div style={{
              background: "rgba(0,212,255,0.07)",
              border: "1px solid rgba(0,212,255,0.15)",
              borderRadius: "20px",
              padding: "4px 14px",
              fontSize: "11px",
              color: "#4a8fa8",
            }}>
              {Object.keys(KNOWLEDGE_BASE).length} INTENTS · O(1) LOOKUP
            </div>
          </div>
        </div>

        {/* STATS ROW */}
        <div style={{
          display: "flex", gap: "24px", marginTop: "14px",
          fontSize: "11px", color: "#3d6b80",
        }}>
          {[
            ["MESSAGES", msgCount],
            ["ARCHITECTURE", "IPO MODEL"],
            ["FALLBACK", "ENABLED"],
            ["SANITIZATION", "ACTIVE"],
          ].map(([k, v]) => (
            <div key={k}>
              <span style={{ color: "#1e4a5c" }}>{k}: </span>
              <span style={{ color: "#00d4ff" }}>{v}</span>
            </div>
          ))}
        </div>
      </div>

      {/* CHAT WINDOW */}
      <div style={{
        width: "100%", maxWidth: "860px",
        flex: 1,
        padding: "0 24px",
        position: "relative", zIndex: 1,
      }}>
        <div style={{
          background: "rgba(0,212,255,0.02)",
          border: "1px solid rgba(0,212,255,0.1)",
          borderRadius: "16px",
          height: "420px",
          overflowY: "auto",
          padding: "20px",
        }}>
          {messages.map((msg, i) => (
            <Bubble key={msg.id} msg={msg} isLast={msg.id === lastBotId} />
          ))}
          <div ref={bottomRef} />
        </div>
      </div>

      {/* SUGGESTIONS */}
      <div style={{
        width: "100%", maxWidth: "860px",
        padding: "14px 24px 0",
        position: "relative", zIndex: 1,
      }}>
        <div style={{
          display: "flex", flexWrap: "wrap", gap: "8px",
          opacity: sessionActive ? 1 : 0.3,
          pointerEvents: sessionActive ? "auto" : "none",
        }}>
          <span style={{ fontSize: "11px", color: "#2a5c6e", alignSelf: "center", marginRight: 4 }}>
            QUICK INTENTS →
          </span>
          {SUGGESTIONS.map(s => (
            <IntentBadge key={s} label={s} onClick={sendMessage} />
          ))}
        </div>
      </div>

      {/* INPUT */}
      <div style={{
        width: "100%", maxWidth: "860px",
        padding: "14px 24px 28px",
        position: "relative", zIndex: 1,
      }}>
        <div style={{
          display: "flex", gap: "12px", alignItems: "flex-end",
          background: "rgba(0,212,255,0.04)",
          border: `1px solid ${sessionActive ? "rgba(0,212,255,0.2)" : "rgba(255,100,100,0.15)"}`,
          borderRadius: "12px",
          padding: "12px 16px",
          transition: "border-color 0.3s",
        }}>
          <div style={{ flex: 1 }}>
            <div style={{ fontSize: "10px", color: "#1e4a5c", marginBottom: "6px", letterSpacing: "2px" }}>
              INPUT → SANITIZE → LOOKUP → OUTPUT
            </div>
            <textarea
              ref={inputRef}
              value={input}
              onChange={e => setInput(e.target.value)}
              onKeyDown={handleKey}
              disabled={!sessionActive}
              placeholder={sessionActive ? "Type your message... (Enter to send, 'exit' to quit)" : "Session terminated. Refresh to restart."}
              rows={1}
              style={{
                width: "100%",
                background: "transparent",
                border: "none",
                color: "#c8eeff",
                fontSize: "14px",
                fontFamily: "'JetBrains Mono', monospace",
                lineHeight: "1.5",
                caretColor: "#00d4ff",
              }}
            />
          </div>
          <button
            onClick={() => sendMessage()}
            disabled={!sessionActive || !input.trim()}
            style={{
              background: sessionActive && input.trim()
                ? "linear-gradient(135deg, #0077ff, #00d4ff)"
                : "rgba(0,212,255,0.08)",
              border: "none",
              borderRadius: "8px",
              padding: "10px 20px",
              color: sessionActive && input.trim() ? "#fff" : "#1e4a5c",
              fontSize: "13px",
              cursor: sessionActive && input.trim() ? "pointer" : "not-allowed",
              fontFamily: "'JetBrains Mono', monospace",
              fontWeight: 600,
              letterSpacing: "1px",
              transition: "all 0.2s",
              whiteSpace: "nowrap",
            }}>
            SEND ↵
          </button>
        </div>

        {/* FOOTER */}
        <div style={{
          display: "flex", justifyContent: "space-between",
          marginTop: "10px",
          fontSize: "10px",
          color: "#1e4a5c",
        }}>
          <span>// DECODELABS · BATCH 2026 · PROJECT 1: RULE-BASED AI CHATBOT</span>
          <span style={{ color: "#0d3040" }}>
            ARCHITECTURE: while True → sanitize → dict.get() → print
          </span>
        </div>
      </div>
    </div>
  );
}