"""Narrow CE migration: only exact operator-proven ignored types are removed."""
import re,xml.etree.ElementTree as ET
def remove_types(text,names):
    before=ET.fromstring(text)
    removed=[]
    def replace(match):
        element=ET.fromstring(match.group())
        name=element.attrib['name']
        if name in names:removed.append(name);return ''
        return match.group()
    changed=re.sub(r'<type\s+name="[^"]+"\s*>.*?</type>',replace,text,flags=re.S)
    after=ET.fromstring(changed)
    original={e.attrib['name']:ET.tostring(e) for e in before.findall('type') if e.attrib['name'] not in names}
    retained={e.attrib['name']:ET.tostring(e) for e in after.findall('type')}
    # Ignore whitespace tails left by removed neighbours, preserve all values.
    def canonical(e):return (e.tag,tuple(sorted(e.attrib.items())),(e.text or '').strip(),tuple(canonical(c) for c in e))
    assert {e.attrib['name']:canonical(e) for e in before.findall('type') if e.attrib['name'] not in names}=={e.attrib['name']:canonical(e) for e in after.findall('type')}
    return changed,removed
