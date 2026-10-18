@_cdecl("print_hello")
func print_hello() {
    print("Hello World!!! from swift")
}

@_cdecl("print_zero_to_ten")
func print_zero_to_ten() {
    for i in 0..<10 {
        print(i)
    }
}

public func print_hello_from_cpp_header() {
    print("Hello World!!! from swift cpp header")
}

public func print_zero_to_ten_from_cpp_header() {
    for i in 0..<10 {
        print(i)
        print("im cpp header")
    }
}
