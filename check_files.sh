for file in *.md; do
  if test "$(head -1 $file)" != '---'; then
    perl -pi -e 'print "---\n---\n\n" if $. == 1' $file;
  fi
  sed -i "s/\.md/.html/g" $file
  sed -i "s/'hidden'/\/*unspecified*\//g" $file
done
