echo $1 
echo $2
emacsclient --eval '(find-file "'$1'")' --eval '(split-window-horizontally)' --eval '(find-file-other-window "'$2'")' --eval '(switch-to-buffer-window "'$1'")' --eval '(switch-to-buffer-other-window "'$2'")'
