variable "aws_region" {
  default = "us-west-2"
}

variable "default_AZ" {
  default = "us-west-2a" #available zone.
}

variable "default_keyname" {
  default = "jasonlue"
}

#networks
variable "vpc_name" {
  default = "PocVpc"
}

variable "cidr_block_main" {
  default = "192.168.188.0/24"
}

# --- /28 = 16 addresses, 0 & 15 reserved. first 4 reserved by amazon
# Network for databases
# HostMin: 192.168.188.5 (first 4 (1,2,3,4) is reserved by amazon)
# HostMax: 192.168.188.14
variable "cidr_block_for_db" {
  default = "192.168.188.0/28"
}

# ---
# Network for app services
# HostMin: 192.168.188.21(first 4 is reserved by amazon)
# HostMax: 192.168.188.30
variable "cidr_block_for_app" {
  default = "192.168.188.16/28"
}

# ---
# Network for LB
# HostMin: 192.168.188.37(first 4 is reserved by amazon)
# HostMax: 192.168.188.46
variable "cidr_block_for_lb" {
  default = "192.168.188.32/28"
}

# ---
# Network for Bastion Host
# HostMin: 192.168.188.53(first 4 is reserved by amazon)
# HostMax: 192.168.188.62
variable "cidr_block_for_bhost" {
  default = "192.168.188.48/28"
}

# ---
# Constant CIDR blocks
variable "cidr_block_internet" {
  default = "0.0.0.0/0"
}

variable "bhost_inst_type" {
  default = "t2.micro"
}

variable "bhost_inst_count" {
  default = 1
}

variable "app_inst_type" {
  default = "t2.micro"
}

variable "app_inst_count" {
  default = 1
}

variable "app_port" {
  default = 10051
}

variable "web_inst_type" {
  default = "t2.micro"
}

variable "web_inst_min_count" {
  default = 1
}

variable "web_inst_max_count" {
  default = 2
}

variable "web_http_port" {
  default = 80
}

variable "web_https_port" {
  default = 443
}

variable "db_inst_type" {
  default = "t2.micro"
}

variable "db_inst_count" {
  default = 1
}

variable "mysql_nofile" {
  default = 10000
}

variable "db_password" {
  default = "mypasswd1"
}

variable "db_port" {
  default = 3306
}

variable "elb_listen_http_port" {
  default = 80
}
