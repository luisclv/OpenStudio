# insert your copyright here

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

require 'erb'

# start the measure
class ReportingMeasureName < OpenStudio::Measure::ReportingMeasure
  # human readable name
  def name
    # Measure name should be the title case of the class name.
    return 'NAME_TEXT'
  end

  # human readable description
  def description
    return 'DESCRIPTION_TEXT'
  end

  # human readable description of modeling approach
  def modeler_description
    return 'MODELER_DESCRIPTION_TEXT'
  end

  # define the arguments that the user will input
  def arguments(model = nil)
    args = OpenStudio::Measure::OSArgumentVector.new

    # bool argument to report report_drybulb_temp
    report_drybulb_temp = OpenStudio::Measure::OSArgument.makeBoolArgument('report_drybulb_temp', true)
    report_drybulb_temp.setDisplayName('Add output variables for Drybulb Temperature')
    report_drybulb_temp.setDescription('Will add drybulb temp and report min/mix value in html.')
    report_drybulb_temp.setValue(true)
    args << report_drybulb_temp

    return args
  end

  # define the outputs that the measure will create
  def outputs
    outs = OpenStudio::Measure::OSOutputVector.new

    # this measure does not produce machine readable outputs with registerValue, return an empty list

    return outs
  end

  # return a vector of IdfObject's to request EnergyPlus objects needed by the run method
  # Warning: Do not change the name of this method to be snake_case. The method must be lowerCamelCase.
  def energyPlusOutputRequests(runner, user_arguments)
    super(runner, user_arguments)

    result = OpenStudio::IdfObjectVector.new

    # To use the built-in error checking we need the model...
    # get the last model and sql file
    model = runner.lastOpenStudioModel
    if model.empty?
      runner.registerError('Cannot find last model.')
      return false
    end
    model = model.get

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    if runner.getBoolArgumentValue('report_drybulb_temp', user_arguments)
      request = OpenStudio::IdfObject.load('Output:Variable,,Site Outdoor Air Drybulb Temperature,Hourly;').get
      result << request
    end

    return result
  end

  # define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)

    # get the last model and sql file
    model = runner.lastOpenStudioModel
    if model.empty?
      runner.registerError('Cannot find last model.')
      return false
    end
    model = model.get

    # use the built-in error checking (need model)
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    # get measure arguments
    report_drybulb_temp = runner.getBoolArgumentValue('report_drybulb_temp', user_arguments)

    # load sql file
    sql_file = runner.lastEnergyPlusSqlFile
    if sql_file.empty?
      runner.registerError('Cannot find last sql file.')
      return false
    end
    sql_file = sql_file.get
    model.setSqlFile(sql_file)

    # put data into the local variable 'output', all local variables are available for erb to use when configuring the input html file
    output =  'Measure Name = ' << name << '<br>'
    output << 'Building Name = ' << model.getBuilding.name.get << '<br>'
    output << 'Floor Area = ' << model.getBuilding.floorArea.to_s << '<br>'
    output << 'Net Site Energy = ' << sql_file.netSiteEnergy.to_s << ' (GJ)<br>'

    # read in template
    html_in_path = "#{File.dirname(__FILE__)}/resources/report.html.in"
    if File.exist?(html_in_path)
      html_in_path = html_in_path
    else
      html_in_path = "#{File.dirname(__FILE__)}/report.html.in"
    end
    html_in = ''
    File.open(html_in_path, 'r') do |file|
      html_in = file.read
    end

    # get the weather file run period (as opposed to design day run period)
    ann_env_pd = nil
    sql_file.availableEnvPeriods.each do |env_pd|
      env_type = sql_file.environmentType(env_pd)
      if env_type.is_initialized
        if env_type.get == OpenStudio::EnvironmentType.new('WeatherRunPeriod')
          ann_env_pd = env_pd
          break
        end
      end
    end

    # if requested look through time series results
    if report_drybulb_temp

      # only try to get the annual timeseries if an annual simulation was run
      if ann_env_pd

        # get desired variable
        key_value = 'Environment'
        time_step = 'Hourly' # "Zone Timestep", "Hourly", "HVAC System Timestep"
        variable_name = 'Site Outdoor Air Drybulb Temperature'
        output_timeseries = sql_file.timeSeries(ann_env_pd, time_step, variable_name, key_value) # key value would go at the end if we used it.

        if output_timeseries.empty?
          runner.registerWarning('Timeseries not found.')
        else
          runner.registerInfo('Found timeseries.')

          output_timeseries = output_timeseries.get.values
          min = nil
          max = nil
          # loop through timeseries and move the data from an OpenStudio timeseries to a normal Ruby array (vector)
          for i in 0..(output_timeseries.size - 1)
            if min.nil? || output_timeseries[i] < min
              min = output_timeseries[i]
            end
            if max.nil? || output_timeseries[i] > max
              max = output_timeseries[i]
            end
          end
          output << 'Minimum Site Outdood Air Drybulb Temperature = ' << min.round(1).to_s << ' (C)<br>'
          output << 'Maximum Site Outdood Air Drybulb Temperature = ' << max.round(1).to_s << ' (C)<br>'

        end
      else
        runner.registerWarning('No annual environment period found.')
      end

    end

    # configure template with variable values
    renderer = ERB.new(html_in)
    html_out = renderer.result(binding)

    # write html file
    html_out_path = './report.html'
    File.open(html_out_path, 'w') do |file|
      file << html_out
      # make sure data is written to the disk one way or the other
      begin
        file.fsync
      rescue StandardError
        file.flush
      end
    end

    # close the sql file
    sql_file.close

    return true
  end
end

# register the measure to be used by the application
ReportingMeasureName.new.registerWithApplication
