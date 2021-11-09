package main

import (
	"fmt"
	"net/http"
)

type Hyndler struct {
	Name string
}

func (h *Hyndler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "Name:", h.Name, "  URL:", r.URL.String())
}

func main() {
	fmt.Println("Server start...")
	someHandler := &Hyndler{Name: "Main page"}
	http.Handle("/main/", someHandler)
	http.HandleFunc("/base/", func(writer http.ResponseWriter, request *http.Request) {
		fmt.Fprint(writer, request.URL.String())
	})
	http.ListenAndServe(":8080", nil)
}
