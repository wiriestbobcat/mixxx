import os
import sys
import requests

API_BASE = "https://api.soundcloud.com"
CLIENT_ID = os.getenv("SOUNDCLOUD_CLIENT_ID")

if not CLIENT_ID:
    print("Set the SOUNDCLOUD_CLIENT_ID environment variable with your SoundCloud API client ID.")
    sys.exit(1)

if len(sys.argv) < 2:
    print("Usage: python soundcloud_api_example.py <search query>")
    sys.exit(1)

query = sys.argv[1]

params = {
    "client_id": CLIENT_ID,
    "q": query,
    "limit": 5,
}

r = requests.get(f"{API_BASE}/tracks", params=params)

if r.status_code != 200:
    print("Request failed", r.status_code, r.text)
    sys.exit(1)

tracks = r.json()
for track in tracks:
    print(f"{track['id']}: {track['title']} by {track['user']['username']}")
