require 'diffy'
require 'colorize'

cwd = File.expand_path(File.dirname(__FILE__))

tests  = cwd+"/tests/"
binary = cwd+"/bin/lurun.exe"

Dir.entries(tests).select do |filename|
  file = tests + filename

  next unless File.file? file
  next unless /^test_.+\.lua$/.match filename

  sol = `lua #{file}`
  res = `#{binary} #{file}c`

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
