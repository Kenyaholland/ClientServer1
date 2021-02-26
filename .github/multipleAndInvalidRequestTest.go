// run with go run .github/testServer.go
package main

import (
	"encoding/xml"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"os/exec"
	"strconv"
	"time"
)

type html struct {
	XMLName xml.Name `xml:"html"`
	Value   string   `xml:"body>p"`
}

func testWithMessage(port int, message string, requests int, url string) int {
	fmt.Printf("\u001b[34mRunning test with %s:\u001b[0m\n", message)
	// Start server and wait a little bit
	cmd := exec.Command("src/serveMessage", strconv.Itoa(port), message)
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		fmt.Println("\u001b[31m", err, "\n\u001b[0m")
		return 1
	}
	defer func() {
		fmt.Println("\u001b[34mOutput of the program:\u001b[0m")
		io.Copy(os.Stdout, stdout)
		fmt.Println()
	}()
	err = cmd.Start()
	if err != nil {
		fmt.Println("\u001b[31m", err, "\n\u001b[0m")
		return 1
	}
	defer func() {
		// Stop process
		if err := cmd.Process.Kill(); err != nil {
			fmt.Println("\u001b[31m", err, "\n\u001b[0m")
		}
	}()
	time.Sleep(300 * time.Millisecond)

	for i := 0; i < requests; i++ {
		// Make http request
		resp, err := http.Get(fmt.Sprintf("http://127.0.0.1:%v/%s", port, url))
		if err != nil {
			fmt.Println("\u001b[31m", err, "\n\u001b[0m")
			return 1
		}
		defer resp.Body.Close()

		// Read response
		body, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			fmt.Println("\u001b[31m", err, "\n\u001b[0m")
			return 1
		}

		// Parse response
		m := &html{}
		err = xml.Unmarshal(body, m)
		if err != nil {
			fmt.Println("\u001b[31m", err, "\n\u001b[0m")
			return 1
		}

		fmt.Printf("Received %s\n", m.Value)
		if m.Value != message {
			fmt.Printf("\u001b[31mFAIL: Received value %s is not expected value %s\n\u001b[0m", m.Value, message)
			return 1
		}
		fmt.Printf("\u001b[32mOK: Received value %s is expected value %s\n\u001b[0m", m.Value, message)
	}

	return 0
}

func main() {
	// Build project
	_, err := exec.Command("c++", "-o", "src/serveMessage", "src/main.cpp").Output()
	if err != nil {
		fmt.Println(err)
	}

	testResult := testWithMessage(35252, "encryptedMessage", 2, "message")
	if testResult != 0 {
		os.Exit(testResult)
	}
	testResult = testWithMessage(3236, "kasdjflkdfjoiclkj3k45jryxcv", 10, "message")
	if testResult != 0 {
		os.Exit(testResult)
	}
	testResult = testWithMessage(5678, "sdfsdfasdfekdsafjlksdfkji", 50, "message")
	if testResult != 0 {
		os.Exit(testResult)
	}
	testResult = testWithMessage(6363, "Not found", 3, "invalidUrl")
	if testResult != 0 {
		os.Exit(testResult)
	}
}
