# Security / repository handling

DeutschZ-ModZ contains proprietary DeutschZ source material.

## Access
This repository is not intended as an open-source distribution. No permission is granted to copy, redistribute, publish, repackage, sell, or reuse the source or assets except where separately authorized by the owner.

## Never commit
- private signing keys (`.biprivatekey`)
- passwords, tokens, Steam credentials, API keys or `.env` files
- live server runtime/profile/storage data
- third-party Workshop dumps or assets without redistribution rights
- generated PBO/bisign build outputs unless an explicit release process requires them

## Signing
Public `.bikey` files belong with deployable server verification material when needed. Private `.biprivatekey` files must remain outside GitHub.

## Incident rule
If a secret is ever committed, deleting the file in a later commit is not enough. Rotate/revoke the secret and remove it from Git history where appropriate.
