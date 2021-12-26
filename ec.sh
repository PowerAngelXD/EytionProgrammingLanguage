echo "welcome to Eytion Project Manager"
echo "choose a option to control(press other keys to quit this script):"
echo "1, build & run"
echo "to be continued";
read INPUT;
if [ $INPUT = "1" ];then 
    echo "eytion builder"
    cmake --build ./build
    echo 'ready for run... do you want to run ?[y/n]'
    read temp
    if [ $temp = "y" ]; then
        ./build/eytion
    fi
fi