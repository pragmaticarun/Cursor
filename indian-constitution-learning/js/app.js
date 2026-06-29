/*
 * app.js
 * ------
 * Front-end logic for the "Learn the Indian Constitution" app.
 * Vanilla JS, no build step. Handles:
 *   - view (page) routing via hash + nav highlighting
 *   - mobile nav toggle and theme (light/dark) toggle
 *   - rendering Overview, Structure and Articles content
 *   - the Flashcards deck
 *   - the multiple-choice Quiz
 */

(function () {
  "use strict";

  const data = window.ConstitutionData;
  const $ = (sel, ctx = document) => ctx.querySelector(sel);
  const $$ = (sel, ctx = document) => Array.from(ctx.querySelectorAll(sel));

  /* ===================== Routing / Navigation ===================== */
  const views = ["overview", "structure", "articles", "flashcards", "quiz"];

  function showView(name) {
    if (!views.includes(name)) name = "overview";
    views.forEach((v) => {
      const el = $("#view-" + v);
      if (el) el.hidden = v !== name;
    });
    $$(".nav-link").forEach((link) => {
      link.classList.toggle("active", link.dataset.nav === name);
    });
    // Lazy-init heavy views the first time they are shown.
    if (name === "flashcards") Flashcards.ensure();
    if (name === "quiz") Quiz.ensure();
    window.scrollTo({ top: 0, behavior: "instant" in window ? "instant" : "auto" });
  }

  function currentHash() {
    return (location.hash || "#overview").replace("#", "");
  }

  window.addEventListener("hashchange", () => {
    showView(currentHash());
    closeMobileNav();
  });

  /* Mobile nav */
  const navToggle = $("#navToggle");
  const mainNav = $("#mainNav");
  function closeMobileNav() {
    mainNav.classList.remove("open");
    navToggle.setAttribute("aria-expanded", "false");
  }
  navToggle.addEventListener("click", () => {
    const open = mainNav.classList.toggle("open");
    navToggle.setAttribute("aria-expanded", String(open));
  });
  $$(".nav-link").forEach((l) => l.addEventListener("click", closeMobileNav));

  /* ===================== Theme toggle ===================== */
  const THEME_KEY = "constitution-theme";
  const themeToggle = $("#themeToggle");
  function applyTheme(theme) {
    document.documentElement.setAttribute("data-theme", theme);
    themeToggle.innerHTML = theme === "dark" ? "&#9790;" : "&#9728;"; // moon / sun
    try { localStorage.setItem(THEME_KEY, theme); } catch (e) {}
  }
  (function initTheme() {
    let saved = null;
    try { saved = localStorage.getItem(THEME_KEY); } catch (e) {}
    if (!saved) {
      saved = window.matchMedia && window.matchMedia("(prefers-color-scheme: dark)").matches ? "dark" : "light";
    }
    applyTheme(saved);
  })();
  themeToggle.addEventListener("click", () => {
    const current = document.documentElement.getAttribute("data-theme");
    applyTheme(current === "dark" ? "light" : "dark");
  });

  /* ===================== Render: Overview ===================== */
  function renderOverview() {
    const facts = $("#factsGrid");
    facts.innerHTML = data.constitutionFacts
      .map(
        (f) =>
          `<div class="fact-card"><div class="fact-label">${esc(f.label)}</div><div class="fact-value">${esc(f.value)}</div></div>`
      )
      .join("");

    $("#preambleText").textContent = data.preamble;
    $("#preambleNotes").innerHTML = data.preambleNotes.map((n) => `<li>${esc(n)}</li>`).join("");
  }

  /* ===================== Render: Structure ===================== */
  function renderStructure() {
    $("#partsGrid").innerHTML = data.parts
      .map(
        (p) => `
        <article class="part-card">
          <div class="part-head">
            <span class="part-no">${esc(p.part)}</span>
            <span class="part-arts">Art. ${esc(p.articles)}</span>
          </div>
          <h3>${esc(p.title)}</h3>
          <p>${esc(p.summary)}</p>
        </article>`
      )
      .join("");

    $("#schedulesGrid").innerHTML = data.schedules
      .map(
        (s) => `
        <article class="schedule-card">
          <span class="sched-no">${esc(s.schedule)}</span>
          <h3>${esc(s.title)}</h3>
          <p>${esc(s.detail)}</p>
        </article>`
      )
      .join("");
  }

  /* ===================== Render: Articles ===================== */
  const Articles = (function () {
    const listEl = () => $("#articlesList");
    const countEl = () => $("#articleCount");
    const searchEl = () => $("#articleSearch");
    const filterEl = () => $("#partFilter");

    function buildFilter() {
      const parts = Array.from(new Set(data.articles.map((a) => a.part)));
      const opts = ['<option value="">All Parts</option>']
        .concat(parts.map((p) => `<option value="${esc(p)}">${esc(p)}</option>`));
      filterEl().innerHTML = opts.join("");
    }

    function matches(article, query, part) {
      if (part && article.part !== part) return false;
      if (!query) return true;
      const hay = [article.number, article.title, article.text, article.explanation, (article.tags || []).join(" ")]
        .join(" ")
        .toLowerCase();
      return hay.includes(query);
    }

    function render() {
      const query = searchEl().value.trim().toLowerCase();
      const part = filterEl().value;
      const filtered = data.articles.filter((a) => matches(a, query, part));

      countEl().textContent = `${filtered.length} article${filtered.length === 1 ? "" : "s"} shown`;

      if (filtered.length === 0) {
        listEl().innerHTML = `<div class="empty-state">No articles match your search. Try a different keyword.</div>`;
        return;
      }

      listEl().innerHTML = filtered
        .map(
          (a) => `
          <details class="article-item">
            <summary class="article-summary">
              <span class="article-heading">
                <span class="article-number">${esc(a.number)}</span>
                <span class="article-title">${esc(a.title)}</span>
              </span>
              <span class="article-meta">
                <span class="article-part-badge">${esc(a.part)}</span>
                <span class="chevron">&#9656;</span>
              </span>
            </summary>
            <div class="article-body">
              <p class="label">What it says</p>
              <p class="article-text">${esc(a.text)}</p>
              <p class="label">In simple terms</p>
              <p class="article-explanation">${esc(a.explanation)}</p>
              ${
                a.tags && a.tags.length
                  ? `<div class="tag-row">${a.tags.map((t) => `<span class="tag">${esc(t)}</span>`).join("")}</div>`
                  : ""
              }
            </div>
          </details>`
        )
        .join("");
    }

    function init() {
      buildFilter();
      searchEl().addEventListener("input", render);
      filterEl().addEventListener("change", render);
      render();
    }
    return { init };
  })();

  /* ===================== Flashcards ===================== */
  const Flashcards = (function () {
    let deck = [];
    let idx = 0;
    let initialized = false;

    function shuffle(arr) {
      const a = arr.slice();
      for (let i = a.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [a[i], a[j]] = [a[j], a[i]];
      }
      return a;
    }

    function show() {
      const card = deck[idx];
      $("#flashFront").textContent = card.front;
      $("#flashBack").textContent = card.back;
      $("#flashcard").classList.remove("flipped");
      $("#flashProgress").textContent = `${idx + 1} / ${deck.length}`;
    }
    function flip() { $("#flashcard").classList.toggle("flipped"); }
    function next() { idx = (idx + 1) % deck.length; show(); }
    function prev() { idx = (idx - 1 + deck.length) % deck.length; show(); }

    function ensure() {
      if (initialized) return;
      initialized = true;
      deck = data.flashcards.slice();
      idx = 0;

      const cardEl = $("#flashcard");
      cardEl.addEventListener("click", flip);
      cardEl.addEventListener("keydown", (e) => {
        if (e.key === " " || e.key === "Enter") { e.preventDefault(); flip(); }
      });
      $("#flashNext").addEventListener("click", next);
      $("#flashPrev").addEventListener("click", prev);
      $("#flashShuffle").addEventListener("click", () => {
        deck = shuffle(data.flashcards);
        idx = 0;
        show();
      });
      show();
    }
    return { ensure };
  })();

  /* ===================== Quiz ===================== */
  const Quiz = (function () {
    let questions = [];
    let idx = 0;
    let score = 0;
    let answered = false;
    let initialized = false;

    const LETTERS = ["A", "B", "C", "D", "E"];

    function shuffle(arr) {
      const a = arr.slice();
      for (let i = a.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [a[i], a[j]] = [a[j], a[i]];
      }
      return a;
    }

    function start() {
      questions = shuffle(data.quizzes);
      idx = 0;
      score = 0;
      answered = false;
      $("#quizResult").hidden = true;
      $("#quizCard").hidden = false;
      renderQuestion();
    }

    function renderQuestion() {
      const q = questions[idx];
      answered = false;
      $("#quizProgress").textContent = `Question ${idx + 1} of ${questions.length}`;
      $("#quizScore").textContent = `Score: ${score}`;
      $("#quizBar").style.width = `${(idx / questions.length) * 100}%`;
      $("#quizQuestion").textContent = q.q;

      const opts = $("#quizOptions");
      opts.innerHTML = q.options
        .map(
          (o, i) =>
            `<button class="quiz-option" data-i="${i}"><span class="opt-marker">${LETTERS[i]}</span><span>${esc(o)}</span></button>`
        )
        .join("");
      $$(".quiz-option", opts).forEach((btn) =>
        btn.addEventListener("click", () => choose(parseInt(btn.dataset.i, 10)))
      );

      const explain = $("#quizExplain");
      explain.hidden = true;
      explain.textContent = "";

      const nextBtn = $("#quizNext");
      nextBtn.disabled = true;
      nextBtn.textContent = idx === questions.length - 1 ? "See Results" : "Next Question";
    }

    function choose(choiceIndex) {
      if (answered) return;
      answered = true;
      const q = questions[idx];
      const buttons = $$(".quiz-option");
      buttons.forEach((b) => {
        const i = parseInt(b.dataset.i, 10);
        b.disabled = true;
        if (i === q.answer) b.classList.add("correct");
        if (i === choiceIndex && choiceIndex !== q.answer) b.classList.add("wrong");
      });
      if (choiceIndex === q.answer) score++;
      $("#quizScore").textContent = `Score: ${score}`;

      const explain = $("#quizExplain");
      const prefix = choiceIndex === q.answer ? "Correct! " : "Not quite. ";
      explain.textContent = prefix + q.explain;
      explain.hidden = false;

      $("#quizNext").disabled = false;
    }

    function next() {
      if (idx < questions.length - 1) {
        idx++;
        renderQuestion();
      } else {
        finish();
      }
    }

    function finish() {
      $("#quizCard").hidden = true;
      const result = $("#quizResult");
      result.hidden = false;
      const total = questions.length;
      const pct = Math.round((score / total) * 100);
      $("#quizFinalScore").textContent = `${score} / ${total}  (${pct}%)`;
      let msg;
      if (pct === 100) msg = "Outstanding! You have an excellent grasp of the Constitution.";
      else if (pct >= 70) msg = "Great job! A solid understanding — review the missed ones to perfect it.";
      else if (pct >= 40) msg = "Good start. Revisit the Articles and Flashcards, then try again.";
      else msg = "Keep learning! Explore the Structure and Articles sections and come back.";
      $("#quizFeedback").textContent = msg;
    }

    function ensure() {
      if (initialized) return;
      initialized = true;
      $("#quizNext").addEventListener("click", next);
      $("#quizRestart").addEventListener("click", start);
      start();
    }
    return { ensure };
  })();

  /* ===================== Utils ===================== */
  function esc(str) {
    return String(str)
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/"/g, "&quot;")
      .replace(/'/g, "&#39;");
  }

  /* ===================== Boot ===================== */
  document.addEventListener("DOMContentLoaded", () => {
    renderOverview();
    renderStructure();
    Articles.init();
    showView(currentHash());
  });
})();
