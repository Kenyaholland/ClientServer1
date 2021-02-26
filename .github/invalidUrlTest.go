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

func testWithMessage(port int, message string, url string) int {
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

	// Make http request
	resp, err := http.Get(fmt.Sprintf("http://127.0.0.1:%v/%s", port, url))
	if err != nil {
		fmt.Println("\u001b[31m", err, "\n\u001b[0m")
		return 1
	}
	defer resp.Body.Close()

	// Status Code
	if resp.StatusCode != 404 {
		fmt.Println("\u001b[31mInvalid status code, received", resp.StatusCode, "instead of 404 \n\u001b[0m")
		return 1
	}

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

	return 0
}

func main() {
	// Build project
	testResult := testWithMessage(61232, "Not found", "asdf")
	if testResult != 0 {
		os.Exit(testResult)
	}
	testResult = testWithMessage(45654, "Not found", "messag")
	if testResult != 0 {
		os.Exit(testResult)
	}
	testResult = testWithMessage(47417, "Not found", "messageA")
	if testResult != 0 {
		os.Exit(testResult)
	}
	testResult = testWithMessage(47418, "Not found", "")
	if testResult != 0 {
		os.Exit(testResult)
	}
}
