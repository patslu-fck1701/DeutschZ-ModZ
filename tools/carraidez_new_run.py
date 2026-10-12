"""Offline operator command. Never connects to or starts a server.
Use only against a downloaded/stopped TEST profile, then upload config + history.
Preserves the current ledger; an active run needs explicit cancellation.
"""
import argparse,json,pathlib,re,uuid,datetime,shutil
VALID=re.compile(r'[A-Za-z0-9_-]{1,64}\Z')
def prepare(root,run_id=None,cancel_active=False):
    root=pathlib.Path(root)
    run_id='operator-'+uuid.uuid4().hex if run_id is None else run_id
    if not VALID.fullmatch(run_id):raise ValueError('Invalid runID')
    config=json.loads((root/'config.json').read_text())
    history=root/'runs'
    if (history/(run_id+'.json')).exists():raise ValueError('Consumed runID')
    ledger_path=root/'ledger.json'
    ledger=json.loads(ledger_path.read_text()) if ledger_path.exists() else None
    if ledger:
        if ledger.get('schemaVersion')!=1 or not VALID.fullmatch(ledger.get('runID','')) or ledger.get('phase') not in ('RESERVED','ACTIVE','CLAIMED','ABORTED') or bool(ledger.get('rewardCommitted'))!=(ledger['phase']=='CLAIMED'):raise ValueError('Invalid ledger: preserve and investigate')
        if ledger['runID']==run_id:raise ValueError('Consumed current runID')
        if ledger['phase'] in ('RESERVED','ACTIVE') and not cancel_active:raise ValueError('Active run: explicit --cancel-active required while stopped')
    stamp=datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    backup=root/'operator-backups'/stamp;backup.mkdir(parents=True)
    shutil.copy2(root/'config.json',backup/'config.json')
    if ledger:
        shutil.copy2(ledger_path,backup/'ledger.json')
        history.mkdir(exist_ok=True)
        target=history/(ledger['runID']+'.json')
        if not target.exists():target.write_text(json.dumps(ledger,indent=2)+'\n')
        if cancel_active and ledger['phase'] in ('RESERVED','ACTIVE'):
            ledger['phase']='ABORTED';ledger['rewardCommitted']=False
            target.write_text(json.dumps(ledger,indent=2)+'\n')
            ledger_path.write_text(json.dumps(ledger,indent=2)+'\n')
    config['runID']=run_id
    (root/'config.json').write_text(json.dumps(config,indent=2)+'\n')
    return run_id
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('profile',type=pathlib.Path)
    parser.add_argument('--run-id');parser.add_argument('--cancel-active',action='store_true')
    parser.add_argument('--server-stopped',action='store_true',required=True)
    args=parser.parse_args()
    print(prepare(args.profile,args.run_id,args.cancel_active))
