"""Read-only log verdict. Never starts a server; no gameplay PASS inferred."""
import argparse,json,pathlib,re
FAIL=re.compile(r'Can.t compile|SCRIPT\s*\(E\)|Broken expression|Multiple declaration|Can.t find variable|Unhandled exception|Exception code:|Crash has occurred',re.I)
def inspect(text):
    failures=[line.strip() for line in text.splitlines() if FAIL.search(line)]
    ready=bool(re.search(r'\[WorldMemoriez\] READY',text))
    grids=re.findall(r'\[WorldMemoriez\] GRID sectors=(\d+) tracked=(\d+) max=(\d+)',text)
    return {'compileOrRuntimeError':bool(failures),'errors':failures,'worldMemoryReady':ready,'grids':grids,'magazineErrors':len(re.findall('unable to create magazine',text,re.I)),'ignoredCETypes':len(re.findall('will be ignored',text)),'gameplayTested':False}
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('logs',nargs='+',type=pathlib.Path);args=parser.parse_args()
    result=inspect('\n'.join(p.read_text(errors='replace') for p in args.logs));print(json.dumps(result,indent=2));raise SystemExit(int(result['compileOrRuntimeError']))
