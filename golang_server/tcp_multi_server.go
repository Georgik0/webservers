package main

import (
	"bufio"
	"fmt"
	"net"
	"strconv"
)

//var pull_workers []int

func worker(conn net.Conn, idx int) {
	name := conn.RemoteAddr().String()

	fmt.Printf("Connect %v worker: %v\n", idx, name)
	conn.Write([]byte("Your number: [" + strconv.Itoa(idx) + "]\n\r"))

	defer conn.Close()

	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		text := scanner.Text()
		fmt.Println("Server get:", text)
		switch text {
		case "exit":
			conn.Write([]byte("Close connection\n\r"))
			fmt.Println(name, "disconnected")
			return
		default:
			conn.Write([]byte("Serevr response: i'm get [" + text + "]\n\r"))
		}
	}
}

func main() {
	listner, err := net.Listen("tcp", ":8081")
	if err != nil {
		panic(err)
	}

	idx := 0
	for {
		conn, err := listner.Accept()
		if err != nil {
			panic(err)
		}
		fmt.Println(idx)
		go worker(conn, idx)
		idx++
	}
}
