require 'ascii_chart'
require 'thwait'

BEGIN {
    class String
        def select(&block) Array(self.each_char).select { |element| block.call element }.join() end
        def select!(&block) self.replace(self.select { |element| block.call element }) end
        def number?() true if Float(self) rescue false end
    end

    module OS
        def OS.windows?() (/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil  end
        def OS.mac?() (/darwin/ =~ RUBY_PLATFORM) != nil end
        def OS.linux?() OS.unix? and not OS.mac? end
        def OS.jruby?() RUBY_ENGINE == 'jruby' end
        def OS.unix?() !OS.windows? end  
    end

    module Echo
        @command = "ping %s -n 1 -w %s" if OS.windows?
        @pattern = /time=\d{1,4}ms/ if OS.windows?

        def Echo.ping(address, timeout = 1000)
            constructed_command = @command % [address, timeout]
            subprocess_pipe = IO.popen(constructed_command)
            command_output = subprocess_pipe.read
            subprocess_pipe.close
            
            latency_match = @pattern.match(command_output)
            return false if latency_match == nil
            
            latency = latency_match[0].select { |character| character.number? }
            Float(latency)
        end
    end

    def AverageLatencyMinute address
        target_time = Time.now.to_i + 60
        measurements = Array.new
    
        until Time.now.to_i > target_time do
            measurements.push Echo.ping address
            sleep(1)
        end
        
        measurements.map! { |element| element.class == Float ? element : 0 }
        measurements.reduce(:+) / measurements.length
    end
    
    def AverageLatencyHour address
        Array(1..60).map{ AverageLatencyMinute address }.reduce(:+) / 60
    end
}

angelic_threads = Hash.new

angelic_threads["averager"] = Thread.new {
    write_average = lambda do |average|
        File.open('hourly_averages.txt', File.exist?('hourly_averages.txt') ? 'a' : 'w+') { |stream| stream.write(average) } 
    end

    loop do
        begin
            hourly_average = AverageLatencyHour '8.8.8.8'
        rescue StandardError => exception
            puts "Encountered, and ignoring exception: #{exception}"
        else
            puts hourly_average
        end
    end
}

END {
    until angelic_threads.length == 0 do
        system(OS.windows? ? 'cls' : 'clear')

        angelic_threads.each do |thread_name, thread|
            if thread.alive?
                puts "(ALIVE) #{thread.object_id}: #{thread_name}"
            else
                puts "(DEAD) #{thread.object_id}: #{thread_name}"
                angelic_threads.delete(thread_name)
            end            
        end

        sleep(1)
    end
}