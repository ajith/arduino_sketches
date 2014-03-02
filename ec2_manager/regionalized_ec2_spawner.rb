#
#
#

require 'rubygems'
require 'aws-sdk'


# Constants
SECURITY_GROUP_NAME = "perf_group"
KEY_PAIR_NAME = "perf_Kp"


def create_regional_keyfile_name(region)
  "/tmp/keys/#{region}-#{KEY_PAIR_NAME}.privatekey"
end
#
# Some utils - move to a different file later
#
def create_sec_group (ec2client)

 filtered =  ec2client.security_groups.filter('group-name',SECURITY_GROUP_NAME)
 count = 0
 filtered.each {|g| count+1}

 if (count == 0)
    secgrp = ec2client.security_groups.create(SECURITY_GROUP_NAME)
    # set the ports per requirement
    secgrp.authorize_ingress :tcp, 22, '0.0.0.0/0'
    
    #secgrp.authorize_egress :tcp, 80, '0.0.0.0/0'
    #secgrp.authorize_egress :tcp, 443, '0.0.0.0/0'
 else
    puts "Sec group #{SECURITY_GROUP_NAME} already exists"
 end
end

def create_key_pair (ec2client,region)
 if (ec2client.key_pairs[KEY_PAIR_NAME].nil?)
    kp = ec2client.key_pairs.create(KEY_PAIR_NAME)
    # save the private key
    File.open(create_regional_keyfile_name(region),"w") do |f|
      f.write(kp.private_key)
    end
 else
    puts "keypair group #{KEY_PAIR_NAME} already exists"
 end
end

#
# These are creds for an IAM user that has power user prevs. In practice, these keys should *NEVER* be
# embedded in code like this
#
creds = {:access_key_id => "AKIAIAUIPNIV6JH5SL7Q", :secret_access_key => "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"}

endpoints = {'us-east-1' => "ec2.us-east-1.amazonaws.com",'us-west-2' => "ec2.us-west-2.amazonaws.com",
'us-west-1' => "ec2.us-west-1.amazonaws.com", 'eu-west-1' => "ec2.eu-west-1.amazonaws.com",
'ap-southeast-1' => "ec2.ap-southeast-1.amazonaws.com",
'ap-southeast-2' => "ec2.ap-southeast-2.amazonaws.com",
'ap-northeast-1' => "ec2.ap-northeast-1.amazonaws.com",
'sa-east-1' => 	"ec2.sa-east-1.amazonaws.com"}


endpoints.each do |k,v|
   ec2 = AWS::EC2.new(creds.merge({:ec2_endpoint => v}))
   
   # create a security group if it does not exist
   create_sec_group(ec2)

   # create a keypair
   create_key_pair(ec2,k)

   
   # create the instance and wait for it to start

   # ssh to the instance and upload some scripts

   # run them


   # terminate them


end


# Create a security group  if the do not exist yet
#


#
#  
#






