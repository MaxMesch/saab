# SAAB Middleware

## Files

- `src/main.cpp` — sets up the middleware and runs the event loop.
- `src/SaabObject.{hpp,cpp}` — object state, the type → category mapping and the color rules.
- `src/ObjectStore.{hpp,cpp}` — keeps the latest state of each unique object id.
- `src/SaabServerClient.{hpp,cpp}` — connects to the SAAB server and parses its lines into the store.
- `src/Protocol.{hpp,cpp}` — encodes the binary report sent to the client.
- `src/StdoutReporter.{hpp,cpp}` — writes a report to stdout every 1 ⅔ seconds.
- `CMakeLists.txt` — build configuration; downloads ASIO 1.39.0 when configuring (needs internet).
- `../decode.py` — test client that decodes the binary reports into readable text.

## Run

From the repo root, start the SAAB server:

```sh
java -classpath SAAB.jar com.saabtech.server.SAABServer
```

In a second terminal, build the middleware and pipe its output into the test client:

```sh
cd middleware
cmake -S . -B build
cmake --build build
./build/saab_middleware | python3 ../decode.py
```
