mkdir libs || true
for i in `find ../../libs -depth 1 -type d`
do
	cp -r $i ./libs
	name=`echo $i | cut -d/ -f4`
	cp ../../libs/VMLib.h libs/$name
	cp VM.h libs/$name
	cp VM_exts.h libs/$name
done
for i in `find libs -type f -name '*.cpp'`
do
	cp $i .
done
for i in `find libs -type f -name '*.h'`
do
        cp $i .
done
