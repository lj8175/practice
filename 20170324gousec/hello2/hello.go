package main

// #include "c_fun.h"  
import "C"

import "fmt"

func main() {
fmt.Println("go call c: 3+4=", C.sun(3, 4))
}
