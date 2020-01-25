# Create separate vpc for project
resource "aws_vpc" "default" {
  cidr_block           = "${var.cidr_block_main}"
  enable_dns_hostnames = true

  tags {
    Name = "${var.vpc_name}"
  }
}

# BEGIN: Network Section

# network to place database instances of our service
resource "aws_subnet" "subnet_for_db" {
  vpc_id                  = "${aws_vpc.default.id}"
  cidr_block              = "${var.cidr_block_for_db}"
  availability_zone       = "${var.default_AZ}"
  map_public_ip_on_launch = true

  tags {
    Name = "Subnet_For_Databases"
  }
}

# network to place app
resource "aws_subnet" "subnet_for_app" {
  vpc_id                  = "${aws_vpc.default.id}"
  cidr_block              = "${var.cidr_block_for_app}"
  availability_zone       = "${var.default_AZ}"
  map_public_ip_on_launch = true

  tags {
    Name = "Subnet_for_Applications"
  }
}

#network to place ELB
resource "aws_subnet" "subnet_for_lb" {
  vpc_id                  = "${aws_vpc.default.id}"
  cidr_block              = "${var.cidr_block_for_lb}"
  availability_zone       = "${var.default_AZ}"
  map_public_ip_on_launch = true

  tags {
    Name = "Subnet_for_LoadBalancers"
  }
}

# Network for bastion host
resource "aws_subnet" "subnet_for_bhost" {
  vpc_id                  = "${aws_vpc.default.id}"
  cidr_block              = "${var.cidr_block_for_bhost}"
  availability_zone       = "${var.default_AZ}"
  map_public_ip_on_launch = true

  tags {
    Name = "Subnet_for_BastionHost"
  }
}

# Enable internet access
resource "aws_internet_gateway" "gw" {
  vpc_id = "${aws_vpc.default.id}"

  tags {
    Name = "${var.vpc_name}_ig"
  }
}

# specify default router
resource "aws_route_table" "r" {
  vpc_id = "${aws_vpc.default.id}"

  route {
    cidr_block = "0.0.0.0/0"
    gateway_id = "${aws_internet_gateway.gw.id}"
  }

  tags {
    Name = "aws_route_table"
  }
}

##
## Add routes to networks
## (first 3 address in every network are reserved for aws services + 1 for subnet)

resource "aws_route_table_association" "rt_app" {
  subnet_id      = "${aws_subnet.subnet_for_app.id}"
  route_table_id = "${aws_route_table.r.id}"
}

resource "aws_route_table_association" "rt_db" {
  subnet_id      = "${aws_subnet.subnet_for_db.id}"
  route_table_id = "${aws_route_table.r.id}"
}

resource "aws_route_table_association" "rt_lb" {
  subnet_id      = "${aws_subnet.subnet_for_lb.id}"
  route_table_id = "${aws_route_table.r.id}"
}

resource "aws_route_table_association" "rt_bhost" {
  subnet_id      = "${aws_subnet.subnet_for_bhost.id}"
  route_table_id = "${aws_route_table.r.id}"
}
