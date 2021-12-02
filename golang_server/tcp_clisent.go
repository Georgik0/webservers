package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
)

func main() {
	conn, _ := net.Dial("tcp", "127.0.0.1:8000")

	defer conn.Close()
	reader_stdin := bufio.NewReader(os.Stdin)


	for {
		go func() {
			fmt.Printf("Input text to send: ")
			text, _ := reader_stdin.ReadString('\n')
			fmt.Fprintf(conn, text)
		}()

		answer, err := bufio.NewReader(conn).ReadString('\n')
		if err != nil {
			break
		}
		fmt.Printf("Answer server: %v", answer)
	}
}
