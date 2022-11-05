import requests
import json
from urllib.parse import urlparse
from urllib.parse import parse_qs

session  = requests.Session()
manifest = open('code-manifest.json')
codeblocks = json.load(manifest)
blobHash = codeblocks["blob"]
blobUrl = f"{codeblocks['url']}/{blobHash}"

def process(block):
    loc = block['Location']
    url = f"{blobUrl}/{loc}"
    parsed_url = urlparse(url)
    fragments = parsed_url.fragment.split('-')
    startLine = int(fragments[0].replace('L',''))
    endLine = int(fragments[1].replace('L',''))
    outFile = block['file']
    with session.get(f"{blobUrl}/{loc}") as resp:
        code = resp.text.split('\n')[startLine:endLine]
        with open(f"snippets/{outFile}.md", 'w', encoding='utf-8') as f:
            f.write(f"[{loc}](")
            f.write(f"https://github.com/pro-bitcoin/pro-bitcoin/blob/{blobHash}/{loc})")
            f.write('{:target="_blank"}\n')
            f.write('```cpp\n')
            for c in code:
                f.write(f"{c}\n")
            f.write('```\n')

for block in codeblocks['blocks']:
    process(block)

manifest.close()



