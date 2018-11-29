# echo "Setting antiNumuCC1piXsecAnalysis v0r0 in /mnt/home/gzarnecki/Highland2/v2r29/highland2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/mnt/home/gzarnecki/T2K/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=antiNumuCC1piXsecAnalysis -version=v0r0 -path=/mnt/home/gzarnecki/Highland2/v2r29/highland2  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

