export function wordCount(text) {
  return (text.toLowerCase().match(/[a-z]+/g) || []).reduce((acc, w) => {
    acc[w] = (acc[w] || 0) + 1;
    return acc;
  }, {});
}