package main

import (
	"net/http"
	"fmt"
)

func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "Server start!")
	w.Write([]byte(":)"))
}

func main() {
	fmt.Println("Server start...")
	http.HandleFunc("/", handler)
	http.HandleFunc("/base/", func(writer http.ResponseWriter, request *http.Request) {
		fmt.Fprint(writer, request.URL.String())
	})
	http.ListenAndServe(":8080", nil)
}
