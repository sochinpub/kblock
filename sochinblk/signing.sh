#
#
#
cd /usr/src/kernels/`uname -r`
scripts/sign-file sha512 signing_key.priv signing_key.x509 /mnt/sdb/kblock/sochinblk/sochin_blk.ko
