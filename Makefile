all:
	standardese --output.advanced_code_block=0 --input.extract_private=0 -I ../jules ../jules/jules
	sh check_files.sh
