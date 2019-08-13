require 'fileutils'

BEGIN {
let_there_be_owos = '
（✿ ͡◕ ᴗ◕)つ━━✫・*。
⊂　　 ノ 　　　・゜+.
しーーＪ　　　°。+ *´¨)
        (¸.·*¨ °。
.· ´      ☆´¨) ¸.·*¨)
'
# Randomly generates a fancy banner of a specified size.
def CreateBanner(target, maximum)
    generated_banner = String.new

    while generated_banner.size <= target && generated_banner.size <= maximum
        generated_banner += ["(¸.·’*", "(¸.·’* *¨)", "(¸.·´"].sample
    end

    return generated_banner
end
}

input_text = nil # The text that will be transformed.
raw_output = false

# Parses each supplied argument, and handles them accordingly.
ARGV.each_with_index do |argument, index|
    if argument.start_with?('-t') and (index + 1) < ARGV.size
        input_text = ARGV[index + 1].dup
    elsif argument.start_with?('-f') and (index + 1) < ARGV.size
        begin
            file_path = ARGV[index + 1]

            input_text = File.read(file_path) unless
                not File.file?(file_path)        
        rescue IOError => exception
            puts "I/O Error when reading file '#{file_path}'"
            exit
        end
    elsif argument == '-r'
        raw_output = true
    end
end

# If the input text is still nil, then no input has been provided
# through command line arguments, and input should therefore be
# read through the standard input instead.
input_text = STDIN.read if input_text.nil?

# If the input text is still nil after reading from STDIN
# then there's some kind of problem with the terminal, and
# the connection to STDIN.
if input_text.nil?
    puts "Nil input after reading from STDIN. There might be a problem with your terminal."
    exit
end

if not raw_output
    puts "|･`) *notices sane speech* what's this? │･ω･`) This is going to nyeed some pwofessionyaw fixing! (・`ω´・) "
    puts let_there_be_owos
end

# SUbstitution table. All substitutions applied one by one.
substitutions = {
    'r' => 'w', # Hurry => Huwwy
    'R' => 'W', # HURRY => HUWWY
    'l' => 'w', # Hello => Hewwo
    'L' => 'W', # HELLO => HEWWO
    /(N|n)([aeiou]|[AEIOU])/ => '\1y\2', # Regex that inserts y's after n's if the following letter is a vowel.
    /(P|p)([aeiou]|[AEIOU])/ => '\1w\2', # Regex that inserts w's after p's if the following letter is a vowel.
}.each do |from, to| 
    input_text.gsub!(from, to)
end

input_text = input_text.chars.map do |character|
    if ['.', '!', '?'].include? character
        character + [
            " ;;w;; ", 
            " owo ", 
            " UwU ", 
            " >w< ", 
            " ^w^ ", 
            " (。U//ω//U。) ", 
            " (◡ ω ◡) ", 
            " (✿◕‿◕) ", 
            " (///∇///✿) ", 
            " (⁄ ⁄◕⁄ω⁄◕⁄ ⁄✿) ", 
            " ( ˃̣̣̥﹏˂̣̣̥ ✿) ", 
            " (◕ㅅ◕✿) ", 
            " (・`ω´・) "
        ].sample
    else
        character
    end
end.join

puts CreateBanner(input_text.size, 50) unless raw_output
puts input_text