#!/usr/bin/perl
use strict;
use warnings;
use CGI;

# Define the directory where files will be saved
my $upload_dir = './uploads';

# Print the HTTP header

# Retrieve form fields
# my $fileName = $ENV("CONTENT_FILENAME");
my $file = $ENV("CONTENT_FILENAME");

open my $fh, '<', $file or die "Cannot open $file";

local *STDIN = $fh;

my $cgi = CGI->new;

# Output form data
print "Content-type: text/html\r\n";
print "Status: 200 OK\r\n";
print "<h1>Form Data</h1>";

# Handle file uploads
my @uploaded_files = $cgi->upload('file');

# Check if files are uploaded
if (@uploaded_files) {
    foreach my $upload (@uploaded_files) {
        my $filename = $cgi->param('file');
        my $filepath = "$upload_dir/" . (split('/', $filename))[-1];
        
        # Save the uploaded file
        open my $out, '>', $filepath or die "Cannot open $filepath: $!";
        while (my $chunk = <$upload>) {
            print $out $chunk;
        }
        close $out;
        
        print "<p>File successfully uploaded: $filepath</p>";
    }
} else {
    print "<p>No files were uploaded.</p>";
}
