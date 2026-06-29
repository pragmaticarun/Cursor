# Learn the Indian Constitution 🇮🇳

An interactive, fully responsive web app to learn the **Constitution of India**. It
explains the overall structure of the Constitution, provides plain-language
explanations of its most important articles, and includes a knowledge check with
**flashcards** and a **quiz**.

The app is built with **plain HTML, CSS and JavaScript** — no build tools, no
frameworks, and no external dependencies. Just open it in a browser.

## ✨ Features

- **Overview** — quick facts, the full Preamble (with key notes), and a short guide.
- **Structure** — all **25 Parts** and **12 Schedules** of the Constitution, each with a concise description.
- **Articles Explained** — detailed, easy-to-read explanations of key articles, with:
  - the gist of "what it says",
  - an "in simple terms" explanation,
  - topic tags,
  - **live search** and **filter by Part**.
- **Flashcards** — flip cards to actively recall important facts; navigate and shuffle the deck.
- **Quiz** — multiple-choice questions with instant feedback, per-question explanations, scoring and a final result.
- **Responsive design** — works on phones, tablets and desktops (hamburger menu on small screens).
- **Light / Dark theme** — auto-detects your system preference and remembers your choice.
- **Accessible** — keyboard support (flip flashcards with Space/Enter), reduced-motion support, and semantic HTML.

## 📂 Folder structure

```
indian-constitution-learning/
├── index.html          # App shell and all page sections
├── css/
│   └── styles.css       # Responsive styling + light/dark theme
├── js/
│   ├── data.js          # All learning content (parts, schedules, articles, flashcards, quiz)
│   └── app.js           # App logic (routing, search, flashcards, quiz, theme)
└── README.md            # This file
```

## 🚀 Running the app

No installation or build step is required.

**Option 1 — open directly**

Open `index.html` in any modern web browser (double-click it, or drag it into a browser tab).

**Option 2 — serve locally (recommended)**

From inside the `indian-constitution-learning/` folder:

```bash
# Python 3
python -m http.server 8000
```

Then visit <http://localhost:8000> in your browser.

## 🧩 How the content is organised

All study content lives in [`js/data.js`](js/data.js), so it is easy to extend:

- `constitutionFacts` — the quick-facts cards on the Overview page.
- `preamble` / `preambleNotes` — the Preamble text and explanatory notes.
- `parts` — the Parts of the Constitution (number, title, article range, summary).
- `schedules` — the Schedules of the Constitution.
- `articles` — the detailed article explanations. Each entry has:
  `number`, `title`, `part`, `tags`, `text` (what it says) and `explanation` (in simple terms).
- `flashcards` — `{ front, back }` question/answer pairs.
- `quizzes` — `{ q, options, answer, explain }` multiple-choice questions (`answer` is the 0-based index of the correct option).

### Adding a new article

Add an object to the `articles` array in `js/data.js`:

```js
{
  number: "Article 51", title: "Promotion of international peace and security",
  part: "Part IV", tags: ["DPSP"],
  text: "The State shall endeavour to promote international peace and security...",
  explanation: "A short, plain-language explanation goes here."
}
```

It will automatically appear in the **Articles** list and be searchable/filterable.

## 📝 A note on accuracy

This project is intended for **education and study**. The explanations are simplified
for learning and may omit nuance. For authoritative legal text, always refer to the
official Constitution of India as published by the Government of India.
