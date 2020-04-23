apt install -y curl
curl -X PUT -H "Content-Type: application/json" -d '{"registration_code":"test-YCTljKf"}' http://localhost:8765/v1/provision
