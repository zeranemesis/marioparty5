import json
j = json.load(open('objdiff.json', encoding='utf-8', errors='replace'))
def show(o, pre=''):
    if isinstance(o, dict):
        for k, v in o.items():
            if k in ('units',):
                print(pre + f'{k}: [{len(v)} units]')
            elif isinstance(v, (dict, list)):
                print(pre + k + ':')
                show(v, pre + '  ')
            else:
                print(pre + f'{k}: {v}')
    elif isinstance(o, list):
        print(pre + f'[{len(o)} items]')
        if o:
            show(o[0], pre + '  ')
show(j)
