"""Generate deployment defaults from the Enforce source, never runtime state."""
import json, pathlib, re
ROOT=pathlib.Path(__file__).resolve().parents[1]
def defaults():
    source=(ROOT/'mods/deutschz_world_memoriez/scripts/3_game/dzwm_settings.c').read_text()
    body=source.split('class DZWM_Settings')[1].split('void Validate')[0]
    result={}
    for typ,key,value in re.findall(r'\b(int|float|bool)\s+(\w+)\s*=\s*([^;]+);',body):
        result[key]= {'true':True,'false':False}.get(value) if typ=='bool' else float(value) if typ=='float' else int(value)
    return result
if __name__=='__main__':
    target=ROOT/'deployment/profiles/DeutschZ-System/deutschz_world_memoriez/config.json'
    target.parent.mkdir(parents=True,exist_ok=True)
    target.write_text(json.dumps(defaults(),indent=2)+'\n')
    print(target)
