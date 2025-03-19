default:
	docker exec -it qnx-build /home/${USER}/qnxprojects/my-project/build.sh

upload:
	sshpass -p "qnxuser" scp ./my-project qnxuser@192.168.1.100:~

run:
	sshpass -p "qnxuser" ssh qnxuser@192.168.1.100 "/data/home/qnxuser/my-project"