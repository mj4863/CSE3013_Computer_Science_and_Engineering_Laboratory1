echo "working directory:"
read dirname

if [ -n "$dirname" ]  
then
    if [ -d $dirname ]
	then	
        cd $dirname
        if [ $? -ne 0 ]
	    then
		    echo "Error: cannot access directory"
		    exit 0
	    fi
	fi
fi

for dir in *
do
    newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"`
    mv $dir $newname
done 