package main

import (
	"bufio"
	"fmt"
	"net"
)

func main() {
	fmt.Println("Start tcp server")

	ln, _ := net.Listen("tcp", ":8081")

	conn, _ := ln.Accept()

	//reader := bufio.NewReader(conn)
	//writer := bufio.NewWriter(conn)
	for {
		message, _ := bufio.NewReader(conn).ReadString('\n')

		fmt.Printf("Server get: %v", message)

		//writer.WriteString("Server get string: " + message)
		conn.Write([]byte(message + "\n"))
	}
}
