#include "pch.hpp"

static thread_local bool8 in_ml_comment = 0;

static void LineProcEofCb() {
  in_ml_comment = 0;
}

static bool CstyleLineProc(std::string_view s) {
  return 0;
}