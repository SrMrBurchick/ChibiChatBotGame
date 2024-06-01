#!/usr/bin/python3
import json
import sys

if len(sys.argv) != 2:
    print("Usage: read_json.py <json_file>")
    sys.exit(1)

json_file = sys.argv[1]

with open(json_file, 'r') as f:
    data = json.load(f)

# Assuming the JSON file contains a single key-value pair like {"MY_MACRO": "value"}
for key, value in data.items():
    print(f'{key}="{value}"')
