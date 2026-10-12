"""Execute a restricted translation of actual pure Enforce rotation helpers.
No scheduler reimplementation and no claim to execute the DayZ VM.
"""
import ast,itertools,pathlib,re,unittest
from types import SimpleNamespace
ROOT=pathlib.Path(__file__).resolve().parents[1]
SOURCE=(ROOT/'mods/deutschz_eventschedulerz/scripts/3_game/dzes_runtime.c').read_text()
class Array(list):
    def Count(self):return len(self)
    def Find(self,value):return self.index(value) if value in self else -1

def extracted(name):
    match=re.search(r'static (?:int|string) '+name+r'\(([^)]*)\)\s*\{',SOURCE)
    start=match.end();level=1;end=start
    while level:
        level += (SOURCE[end]=='{')-(SOURCE[end]=='}');end+=1
    params=[p.strip().split()[-1] for p in match.group(1).split(',') if p.strip()]
    lines=['def '+name+'('+','.join(params)+'):'];indent=1
    body=re.sub(r'(if[^\n]+\))\s*\n\s*(return[^;]+;)',r'\1 \2',SOURCE[start:end-1])
    for line in body.splitlines():
        line=line.strip()
        if not line:continue
        if line=='{':indent+=1;continue
        if line=='}':indent-=1;continue
        line=re.sub(r'^(int|string) ','',line).replace('||',' or ').replace('&&',' and ')
        line=re.sub(r'!(?!=)','not ',line)
        if line.startswith('if '):
            opening=line.index('(');depth=1;closing=opening+1
            while depth:
                depth += (line[closing]=='(')-(line[closing]==')');closing+=1
            rest=line[closing:].strip().rstrip(';')
            line='if '+line[opening+1:closing-1]+':'+(' '+rest if rest else '')
        else:line=line.rstrip(';')
        line=re.sub(r'(\w+)\+\+',r'\1 += 1',line)
        lines.append('    '*indent+line)
    tree=ast.parse('\n'.join(lines))
    allowed=(ast.Module,ast.FunctionDef,ast.arguments,ast.arg,ast.If,ast.Return,ast.Assign,ast.AugAssign,ast.Expr,ast.Name,ast.Attribute,ast.Call,ast.Constant,ast.Load,ast.Store,ast.BoolOp,ast.Or,ast.And,ast.UnaryOp,ast.Not,ast.USub,ast.Compare,ast.Eq,ast.NotEq,ast.Gt,ast.GtE,ast.Lt,ast.LtE,ast.BinOp,ast.Add,ast.Sub,ast.Subscript)
    assert all(isinstance(node,allowed) for node in ast.walk(tree))
    return compile(tree,'Enforce:'+name,'exec')
def environment():
    rotation=re.search(r's_MajorRotation = \{([^}]+)\}',SOURCE)
    env={'__builtins__':{},'s_MajorRotation':Array(re.findall(r'"([^"\n]+)"',rotation.group(1))),'s_State':SimpleNamespace(RotationIndex=0),'s_Queue':Array()}
    for name in ['RotationIndexOf','RotationOwner','ResolveRotationIndex','NextRotationIndex','SelectQueuedOwner']:exec(extracted(name),env)
    return env
class ActualRotationTests(unittest.TestCase):
    def test_four_complete_rotations_for_all_registration_orders(self):
        for registration in itertools.permutations(['CONVOY','KOTH','RAVEN','COURIER']):
            env=environment();env['s_Queue'].extend(registration)
            grants=[]
            for _ in range(16):
                owner=env['SelectQueuedOwner']();self.assertTrue(owner);grants.append(owner)
                env['s_Queue'].remove(owner)
                self.assertNotEqual(env['SelectQueuedOwner'](),owner)
                env['s_State'].RotationIndex=env['NextRotationIndex'](owner,env['s_State'].RotationIndex)
                env['s_Queue'].append(owner)
            self.assertEqual(grants,['KOTH','COURIER','RAVEN','CONVOY']*4)
    def test_missing_provider_cannot_be_bypassed_by_queue_refresh(self):
        env=environment();env['s_Queue'].extend(['CONVOY','RAVEN','COURIER'])
        self.assertEqual(env['SelectQueuedOwner'](),'')
        env['s_Queue'].append('KOTH');self.assertEqual(env['SelectQueuedOwner'](),'KOTH')
    def test_restore_provider_and_next_owner_precedence(self):
        env=environment()
        for state,want in [(SimpleNamespace(NextEvent='RAVEN',ProviderId='KOTH',LastEvent='CONVOY'),2),(SimpleNamespace(NextEvent='',ProviderId='COURIER',LastEvent='KOTH'),1),(SimpleNamespace(NextEvent='',ProviderId='',LastEvent='CONVOY'),0),(None,0)]:self.assertEqual(env['ResolveRotationIndex'](state),want)
    def test_invalid_saved_rotation_is_bounded(self):
        env=environment()
        self.assertEqual(env['RotationOwner'](-10),'KOTH');self.assertEqual(env['RotationOwner'](99),'CONVOY')
        self.assertEqual(env['NextRotationIndex']('unknown',99),0)
if __name__=='__main__':unittest.main()
