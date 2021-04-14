if [ "$DEBUG_AS_ROOT" = "true" ]; then
    DLV=$(which dlv)
#    echo $(whoami)
#    echo exec sudo "$DLV" --only-same-user=false "$@"
    exec sudo "$DLV" --only-same-user=false "$@"
else
#    echo dlv "$@"
    exec dlv "$@"
fi
