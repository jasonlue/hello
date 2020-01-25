variable "aws_profile"{
    description="AWS profile to use"
    default="default"
}
provider "aws"{
    region="us-west-2"
    profile="${var.aws_profile}"
}

terraform{
    backend "s3"{
        bucket = "terraform.jasonlue.github.io"
        key="hello.tfstate"
        region="us-west-2"
    }
}