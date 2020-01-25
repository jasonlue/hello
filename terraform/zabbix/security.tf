# BEGIN: Sec Groups
# allow internet access from this computer
resource "aws_security_group" "sg_allow_internet" {
  name        = "sg_allow_internet"
  description = "allow all outgoing traffic"
  vpc_id      = "${aws_vpc.default.id}"

  egress {
    from_port   = 0                              # 0 = all
    to_port     = 0                              # 0 = all
    protocol    = "-1"                           # -1 = all, icmp, tcp,udp, -1 or other protocol number
    cidr_blocks = ["${var.cidr_block_internet}"] #destination
  }
}

#allow ssh access to this computer from bastion host
resource "aws_security_group" "sg_bastion" {
  name        = "sg_bastion"
  description = "allow ssh traffic from bastion host"
  vpc_id      = "${aws_vpc.default.id}"

  ingress {
    from_port   = 22                              //ssh port
    to_port     = 22
    protocol    = "tcp"
    cidr_blocks = ["${var.cidr_block_internet}"] //source
  }
}

# allow admin from bhost to access this computer
resource "aws_security_group" "sg_admin" {
  name        = "sg_admin"
  description = "allow all traffic from bastion host"
  vpc_id      = "${aws_vpc.default.id}"

  ingress {
    from_port   = 0
    to_port     = 0
    protocol    = -1
    cidr_blocks = ["${var.cidr_block_for_bhost}"]
  }
}

# load balancer 
resource "aws_security_group" "sg_lb2app" {
  name        = "sg_lb2app"
  description = "allow traffic from load balancer and to load balancer"
  vpc_id      = "${aws_vpc.default.id}"

  ingress {
    from_port   = "${var.web_http_port}"
    to_port     = "${var.web_http_port}"
    protocol    = "tcp"
    cidr_blocks = ["${var.cidr_block_for_lb}"]
  }

  ingress {
    from_port   = "${var.web_https_port}"
    to_port     = "${var.web_https_port}"
    protocol    = "tcp"
    cidr_blocks = ["${var.cidr_block_for_lb}"]
  }
}

resource "aws_security_group" "sg_elb" {
  name        = "sg_elb"
  description = "elb security group"
  vpc_id      = "${aws_vpc.default.id}"

  ingress {
    from_port   = "${var.elb_listen_http_port}"
    to_port     = "${var.elb_listen_http_port}"
    protocol    = "tcp"
    cidr_blocks = ["${var.cidr_block_internet}"]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["${var.cidr_block_internet}"]
  }
}

resource "aws_security_group" "sg_app2db" {
  name        = "sg_app2db"
  description = "allow traffic from app to db"
  vpc_id      = "${aws_vpc.default.id}"

  ingress {
    from_port   = "${var.db_port}"
    to_port     = "${var.db_port}"
    protocol    = "tcp"
    cidr_blocks = ["${var.cidr_block_for_app}"]
  }
}

# ?? 
resource "aws_security_group" "sg_web2app" {
  name        = "sg_web2app"
  description = "allow web to app"
  vpc_id      = "${aws_vpc.default.id}"

  ingress {
    from_port   = "${var.app_port}"
    to_port     = "${var.app_port}"
    protocol    = "tcp"
    cidr_blocks = ["${var.cidr_block_for_app}"]
  }
}

# END: Sec Groups

