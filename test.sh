./src/cmake-build-debug/templater \
    --variables="{\"DEFAULT_PATH\":\"/usr/opt/web\", \"VAR_FROM_TEMPLATE\":\"Hello, C++ world!\"}" \
    --input=./example/input_text.json \
    --output=./example/output_text.json