import json
j = json.load(open('objdiff.json', encoding='utf-8', errors='replace'))
u = [x for x in j.get('units', []) if 'saveload' in x.get('name','')]
print(json.dumps(u, indent=2))
