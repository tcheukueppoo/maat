package main

import (
   "fmt"
)

var ast      = make([]any, 0)
var prefix   = []string{"+", "-"}
var op_codes = map[int8]any {
                              0: []string{ "+", "-" },
                              1: "/",
                              2: "*",
                              3: "(",
                            }

// 2 * + 4 / 2 - 1 --> ((2 * (+4)) / 2) - 1
func main() {
   expr := []any{2, "*", "+", 4, "/", 2, "-", 1}


}
