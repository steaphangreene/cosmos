#!/bin/csh -f

foreach f ($argv)
	if(`grep '} gimp_image = {' $f` == "") then
		echo "$f is not a gimp c file"
	else
		set b=`echo $f | sed 's-\.c$--g'`
		set o=${b}.h
		echo "$f -> $o"
		cat $f | sed 's-} gimp_image = {-} '$b'_image = {-g' > $o
	endif
end
