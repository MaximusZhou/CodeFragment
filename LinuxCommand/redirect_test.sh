#ÖØ¶¨Ïò²âÊÔ
# open three more fds for funnelling information of various levels
exec 3>~/temp/debug.log
exec 4>~/temp/verbose.log
exec 5>~/temp/info.log

echo >&3 I am a debug message
echo >&4 I am a verbose message
echo >&5 I am an info message

# close them after we finish
exec 5>&-
exec 4>&-
exec 3>&-
