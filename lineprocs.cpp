#include "pch.hpp"

static thread_local bool8 in_ml_comment = 0;

static void LineProcEofCb() {
  in_ml_comment = 0;
}

static bool IsWhitespace(s8 c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' || c == '\v';
}

static bool IsLineBlank(std::string_view line) {
  for (s8 c : line) {
    if (!IsWhitespace(c)) {
      return 0;
    }
  }
  return 1;
}

static bool32 SvStartsWith(std::string_view s, const s8* prefix) {
  size_t len = strlen(prefix);
  if (len > s.size()) {
    return 0;
  }
  for (size_t i = 0; i < len; ++i) {
    if (s[i] != prefix[i]) {
      return 0;
    }
  }
  return 1;
}

static bool32 BatLineProc(std::string_view s) {
  if (SvStartsWith(s, "REM")
    || SvStartsWith(s, "rem")
    || SvStartsWith(s, "::")) {
    return 1;
  }
  else {
    return 0;
  }
}

static bool32 CstyleLineProc(std::string_view s) {
  //NOTE:This is missing multi-line string literals
  // but those vary from language to language so could
  // be hard to do. Also could go wrong with c# interpolated
  // strings

  // Could also add support for c/c++ style comment
  // #if 0
  // #endif
  bool32 sl_comm = SvStartsWith(s, "//");
  bool32 str_lit = 0;
  for (size_t i = 1; i < s.size(); ++i) {
    // if a " is encountered outside a comment
    if (s[i] == '"' && !sl_comm && !in_ml_comment) {
      // toggle if not inside a string lit or if the prev char is not an escape
      if (!str_lit || s[i - 1] != '\\') {
        str_lit = !str_lit;
      }
    }

    if (str_lit) continue;

    if (s[i - 1] == '/' && s[i] == '*') {
      in_ml_comment = 1;
    }

    if (s[i - 1] == '*' && s[i] == '/') {
      in_ml_comment = 0;
    }
  }
  bool32 comm = in_ml_comment || sl_comm;
  return comm;
}