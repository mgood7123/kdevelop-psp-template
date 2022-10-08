PSPSDK="$(psp-config --pspsdk-path)"

# create an array from line based output
mapfile -t found_files < <(find src -type f)

# filter found files with wanted files
mapfile -t files < <(
    printf "../%s\n" ${found_files[*]} | grep "\.S$" | sed s/.S$/.o/g
    printf "../%s\n" ${found_files[*]} | grep "\.c$" | sed s/.c$/.o/g
    printf "../%s\n" ${found_files[*]} | grep "\.cpp$" | sed s/.cpp$/.o/g
)

# iterate through array of wanted files
len=${#files[@]}

if [[ $len == 0 ]]
    then
        echo "NO FILES FOUND!"
        exit
fi

cd required

if [[ -e makefile_psp ]]
    then
        rm makefile_psp
fi

touch makefile_psp

echo "TARGET = %{APPNAMEID}" >> makefile_psp

printf "OBJS = " >> makefile_psp

if [[ $len == 1 ]]
    then
        echo -e "${files[0]}" >> makefile_psp
    else
        echo -e "${files[0]} \\" >> makefile_psp
        for (( i=1; i<${len}; i++ )); do
            if [[ $i != $(($len-1)) ]]
                then
                    echo -e "\t ${files[$i]} \\" >> makefile_psp
                else
                    echo -e "\t ${files[$i]}" >> makefile_psp
            fi
        done
fi
echo >> makefile_psp
echo "BUILD_PRX = 1" >> makefile_psp
echo "PRX_EXPORTS=../exports.exp" >> makefile_psp
echo "LDFLAGS = -nostartfiles" >> makefile_psp
echo "LIBS = -lpspctrl_driver -lpspdisplay_driver -lpsppower_driver" >> makefile_psp
echo "INCDIR = include" >> makefile_psp

echo >> makefile_psp
echo "PSPSDK = $PSPSDK" >> makefile_psp
echo "include $PSPSDK/lib/build.mak" >> makefile_psp
