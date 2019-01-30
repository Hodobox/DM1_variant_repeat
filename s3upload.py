import boto3, os
import progressbar

bucket_name = "xilab"
folder_name = 'cancer-cell-line'
file_name = "yang38.gtf"
path = folder_name + "/" + file_name
s3 = boto3.client('s3', aws_access_key_id="AKIAJIRCDTJUBEH2KBSA", aws_secret_access_key="odMNKwxInPG5DsA8A+WAQKCvXyNoyam6ftKPcEF/")
# You get the filesize from wherever you have the file on. your system maybe?

statinfo = os.stat(file_name)



#with open(file_name, "w") as f:
#    filesize = size(f.size())
#    print(filesize)

up_progress = progressbar.progressbar.ProgressBar(maxval=statinfo.st_size)

up_progress.start()

def upload_progress(chunk):
    up_progress.update(up_progress.currval + chunk)

s3.upload_file(file_name, bucket_name, path, Callback=upload_progress)

up_progress.finish()