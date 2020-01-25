provider "aws" {
  region = "${var.aws_region}"

  #shared_credentials_file = "/Users/jason/.aws/credentials"
  profile = "default"
}

terraform {
  backend "s3" {
    bucket = "terraform.jasonlue.github.io"
    key    = "zabbix.tfstate"
    region = "us-west-2"
  }
}
