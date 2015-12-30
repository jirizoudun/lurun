require 'diffy'
require 'colorize'

cwd = File.expand_path(File.dirname(__FILE__))

tests  = cwd+"/tests/"
binary = cwd+"/bin/lurun.exe"


def cleanup(filename)
  clfile = filename.chomp(".lua") + ".cleanup"
  return unless File.file? clfile
  File.open(clfile).each do |line|
    next if /[\/\\]/.match line
    
    del = File.expand_path(line.strip)
    next unless File.file? del
    
    print "deleting "+del+"\n"
    File.delete del 
  end
end

Dir.chdir(tests)

Dir.entries(".").select do |filename|
  file = filename

  next unless File.file? file
  next unless /^test_.+\.lua$/.match filename

  sol = `lua #{file}`
  cleanup filename

  res = `#{binary} #{file}c`
  cleanup filename

  diff = Diffy::Diff.new(sol, res)
  success = diff.to_s == ""

  print "["
  print "FAILURE".red unless success
  print "SUCCESS".green   if success
  print "] "
  print filename.chomp(".lua")
  print "\n"

  print diff.to_s + "\n" unless success
end
