
resource "aws_instance" "hello" {
    ami="ami-01e0cf6e025c036e4"
    #ami = "ami-2757f631" #ubuntu 16.04 LTS
 #   ami = "ami-b374d5a5" #ubuntu 16.10
    instance_type = "t2.micro"
    depends_on = ["aws_s3_bucket.hello"]

    provisioner "local-exec"{
        command = "echo ${aws_instance.hello.public_ip} > ip_address.txt"
    }
}

resource "aws_eip" "ip"{
    instance = "${aws_instance.hello.id}"
}

resource "aws_s3_bucket" "hello"{
    bucket = "hello.jasonlue.github.io"
    acl = "private"
}

