<%
     
FP_SetLocaleForPage

' determine whether or not to provide navigation controls
if fp_iPageSize > 0 then
	fp_fShowNavbar = True
else
	fp_fShowNavbar = False
end if

fp_sPagePath = Request.ServerVariables("PATH_INFO")
fp_sEnvKey = fp_sPagePath & "#fpdbr_" & fp_iRegion
fp_sFormName = "fpdbr_" & CStr(fp_iRegion)
fp_sFormKey = fp_sFormName & "_PagingMove"

fp_sInputs = fp_sDefault

fp_DEBUG = False

fp_sFirstLabel = "  |<  "
fp_sPrevLabel  = "   <  "
fp_sNextLabel  = "  >   "
fp_sLastLabel  = "  >|  "
fp_sDashLabel  = "  --  "

if not IsEmpty(Request(fp_sFormKey)) then
	fp_sMoveType = Request(fp_sFormKey)
else
    fp_sMoveType = ""
end if

fp_iCurrent=1
fp_fError=False
fp_bBlankField=False
Set fp_dictInputs = Server.CreateObject("Scripting.Dictionary")
Set fp_dictParams = Server.CreateObject("Scripting.Dictionary")
Set fp_dictColTypes = Server.CreateObject("Scripting.Dictionary")
fp_iParam = 1

fp_sQry = FP_ReplaceQuoteChars(fp_sQry)

' replace any input parameters in query string
' there need to be at least 5 more characters in the string for there to be input parameters (::[_a-z]::)
Do While (Not fp_fError) And (fp_iCurrent + 5 < Len(fp_sQry) And Instr(fp_iCurrent, fp_sQry, "::") > 0)
	fp_iMax = Len(fp_sQry) + 1
	fp_iColonStart = Instr(fp_iCurrent, fp_sQry, "::")
	fp_iSQuoteStart = Instr(fp_iCurrent, fp_sQry, "'")
	fp_iDQuoteStart = Instr(fp_iCurrent, fp_sQry, """")
	
	If (fp_iSQuoteStart = 0) then
		fp_iSQuoteStart = fp_iMax
	End If
	If (fp_iDQuoteStart = 0) then
		fp_iDQuoteStart = fp_iMax
	End If
	
	fp_sQuoteDelim = ""
	fp_iQuoteStart = -1
	fp_iQuoteEnd = fp_iMax
	fp_bQuoteFound = false
	If (fp_iColonStart > fp_iSQuoteStart and fp_iDQuoteStart > fp_iSQuoteStart) then 'single quote is first sought for character
		fp_sQuoteDelim = "'"
		fp_iQuoteStart = fp_iSQuoteStart
	elseIf (fp_iColonStart > fp_iDQuoteStart and fp_iSQuoteStart > fp_iDQuoteStart) then 'double quote is first sought for character
		fp_sQuoteDelim = """"
		fp_iQuoteStart = fp_iDQuoteStart
	else
		'The :: comes before any ' or "
	End If

	If(fp_sQuoteDelim <> "") then
		fp_iPotQuoteEnd = fp_iQuoteStart + 1
		Do While (fp_bQuoteFound = false and fp_iPotQuoteEnd < fp_iMax)
			fp_iPotQuoteEnd = Instr(fp_iPotQuoteEnd, fp_sQry, fp_sQuoteDelim)

			If(fp_iPotQuoteEnd = 0) then 
				exit do
			End If

			If(fp_iPotQuoteEnd = fp_iMax - 1) then
				fp_iQuoteEnd = fp_iPotQuoteEnd
				fp_bQuoteFound = true
				exit do
			End If
            
			If(Mid(fp_sQry, fp_iPotQuoteEnd + 1, 1) <> fp_sQuoteDelim) then
				fp_iQuoteEnd = fp_iPotQuoteEnd
				fp_bQuoteFound = true
			else
				fp_iPotQuoteEnd = fp_iPotQuoteEnd + 2
			End If
		Loop
	    
		If(fp_bQuoteFound = false) then
			Err.Description = "Unable to find a matching quote for " & fp_sQuoteDelim & " in the query."
			fp_fError = true
			fp_bSkip = true
		End If
	    
		If(fp_iColonStart > fp_iQuoteEnd) then 'there is no user input in this literal string
			fp_iCurrent = fp_iQuoteEnd + 1
			fp_bSkip = true
		End If
	    
	else
		fp_iQuoteStart = fp_iColonStart
		fp_bQuoteFound = false
	End If
	
	If not fp_bSkip then
		fp_iStart = fp_iColonStart
		' found a opening ::, find the close ::
		fp_iEnd = InStr(fp_iStart + 2, fp_sQry, "::")
	
		If not fp_bQuoteFound then
			fp_iQuoteEnd = fp_iEnd + 1
		End If
		If fp_iEnd = 0 Then
			fp_fError = True
			Response.Write "<B>Database Results Error: mismatched parameter delimiters</B>"
		Else
			fp_sField = Mid(fp_sQry, fp_iStart + 2, fp_iEnd - fp_iStart - 2)
			fp_sValue = Request.Form(fp_sField)
			if len(fp_sValue) = 0 then fp_sValue = Request.QueryString(fp_sField)

			' if the named form field doesn't exist, make a note of it
			If (len(fp_sValue) = 0) Then
				fp_iStartField = InStr(fp_sDefault, fp_sField & "=")
				if fp_iStartField > 0 then
					fp_iStartField = fp_iStartField + len(fp_sField) + 1
					fp_iEndField = InStr(fp_iStartField,fp_sDefault,"&")
					if fp_iEndField > 0 then
						fp_sValue = Mid(fp_sDefault,fp_iStartField,fp_iEndField - fp_iStartField)
					else
						fp_sValue = Mid(fp_sDefault,fp_iStartField)
					end if
				end if
			End If
	
			' remember names and values used in query
			if not fp_dictInputs.Exists(fp_sField) then
				fp_dictInputs.Add fp_sField, fp_sValue
			end if
		
			if (len(fp_sValue) = 0) Then fp_bBlankField = True
			
			fp_iOpEnd = fp_iQuoteStart - 1
		
			Do While (Mid (fp_sQry , fp_iOpEnd , 1) = " ")
				fp_iOpEnd = fp_iOpEnd - 1
			Loop
		
			fp_iFieldEnd = fp_iOpEnd
			If ( Mid(fp_sQry, fp_iOpEnd - 1, 2) = "<=") then
				fp_iFieldEnd = fp_iOpEnd - 2
			ElseIf (Mid(fp_sQry, fp_iOpEnd - 1, 2) = ">=") then
				fp_iFieldEnd = fp_iOpEnd - 2
			ElseIf (Mid(fp_sQry, fp_iOpEnd - 1, 2) = "<>") then
				fp_iFieldEnd = fp_iOpEnd - 2
			ElseIf (UCase(Mid(fp_sQry, fp_iOpEnd - 3, 4)) = "LIKE" ) then
				fp_iFieldEnd = fp_iOpEnd - 4
			ElseIf (Mid(fp_sQry, fp_iOpEnd, 1) = "=") then
				fp_iFieldEnd = fp_iOpEnd - 1
			ElseIf (Mid(fp_sQry, fp_iOpEnd, 1) = "<") then
				fp_iFieldEnd = fp_iOpEnd - 1
			ElseIf (Mid(fp_sQry, fp_iOpEnd, 1) = ">") then
				fp_iFieldEnd = fp_iOpEnd - 1
			End If
		
			If(fp_iFieldEnd <> fp_iOpEnd) Then
				Do While (fp_iFieldEnd) > 0 and (Mid(fp_sQry,fp_iFieldEnd,1) = " ")
					fp_iFieldEnd = fp_iFieldEnd - 1
				Loop
				fp_colNameDelim = ""
			
				If(fp_iFieldEnd) > 0 then
					fp_sTemp = Mid(fp_sQry,fp_iFieldEnd,1)

					If(InStr("])""",fp_sTemp)) then
						If(InStr("]",fp_sTemp)) then
							fp_colNameDelim = ".["
						ElseIf (InStr(")",fp_sTemp)) then
							fp_colNameDelim = ".("
						ElseIf (InStr("""",fp_sTemp)) then
							fp_colNameDelim = "."""
					End If
						'In the End, we ignore the 'quote' character
					fp_iFieldEnd = fp_iFieldEnd - 1
				End If
			End If
			       
			fp_iFieldStart = fp_iFieldEnd
			If (fp_colNameDelim = "") then
				fp_colNameDelim = " (."
			End If
			
			DO while (fp_iFieldStart > 0 and InStr(fp_colNameDelim, Mid(fp_sQry,fp_iFieldStart,1)) = 0) 
			    fp_iFieldStart = fp_iFieldStart - 1
			Loop
			
			fp_iFieldStart = fp_iFieldStart + 1
	
			fp_sColName = Mid(fp_sQry, fp_iFieldStart, fp_iFieldEnd - fp_iFieldStart + 1)
			
			If( "NOT" = UCase(fp_sColName)) then
				fp_iFieldEnd = fp_iFieldStart - 1
				Do While (fp_iFieldEnd) > 0 and (Mid(fp_sQry,fp_iFieldEnd,1) = " ")
				    fp_iFieldEnd = fp_iFieldEnd - 1
				Loop
			
				fp_colNameDelim = ""
				
				If(fp_iFieldEnd) > 0 then
				   fp_sTemp = Mid(fp_sQry,fp_iFieldEnd,1)
	
					If(InStr("])""",fp_sTemp)) then
						If(InStr("]",fp_sTemp)) then
				       		fp_colNameDelim = ".["
						ElseIf (InStr(")",fp_sTemp)) then
							fp_colNameDelim = ".("
						ElseIf (InStr("""",fp_sTemp)) then
							fp_colNameDelim = "."""
						End If
						'In the End, we ignore the 'quote' character
						fp_iFieldEnd = fp_iFieldEnd - 1
					End If
				End If
				       
				fp_iFieldStart = fp_iFieldEnd
				If(fp_colNameDelim = "") Then
					fp_colNameDelim = " (."
				End If
				
				Do while (fp_iFieldStart > 0 and InStr(fp_colNameDelim, Mid(fp_sQry,fp_iFieldStart,1)) = 0) 
					fp_iFieldStart = fp_iFieldStart - 1
				Loop			
				fp_iFieldStart = fp_iFieldStart + 1

				fp_sColName = Mid(fp_sQry, fp_iFieldStart, fp_iFieldEnd - fp_iFieldStart + 1)
			End If

			fp_sColName = Replace(fp_sColName, "[", "")
			fp_sColName = Replace(fp_sColName, "]", "")

			fp_colType = ""
			fp_iStartField = InStr(fp_sColTypes, "&" & fp_sColName & "=")
			If fp_iStartField > 0 Then
				fp_iStartField = fp_iStartField + len(fp_sColName) + 2
				fp_iEndField = InStr(fp_iStartField,fp_sColTypes,"&")
				If fp_iEndField > 0 Then
					fp_colType = Mid(fp_sColTypes,fp_iStartField,fp_iEndField - fp_iStartField)
				else
					Err.Description = "fp_sColTypes appears to be malformed.<br>"
					Err.Description = Err.Description & "This could happen if your DatabaseRegionStart webbot has an empty or missing s-columnnames or s-columntypes attributes.<br>You may need to read Microsoft Knowledge Base Article 817029.<br>"
					fp_fError = true
				End If
			End If

			If(Len(fp_colType) > 0 and IsNumeric(fp_colType)) Then
				fp_dictColTypes.Add fp_iParam, fp_colType
				
				'Remove single quotes around strings
				select case fp_colType
					case 129, 200, 201, 130, 202, 203 ' adChar, adVarChar, adLongVarChar, adWChar, adVarWChar, adLongVarWChar
						If not fp_bQuoteFound and Left(fp_sValue, 1) = "'" or Left(fp_sValue, 1) = """" Then
							fp_sValue = Mid(fp_sValue,2,Len(fp_sValue)-2)		
						End If
					case else
						' do nothing
				End select
                
				If fp_sQuoteDelim =  """"  Then
					fp_sValue = Replace(fp_sValue, """""", """")
				ElseIf fp_sQuoteDelim = "'" Then
					fp_sValue = Replace(fp_sValue, "''", "'")
				End If

				If (fp_bQuoteFound) then
					fp_sLead = Mid(fp_sQry, fp_iQuoteStart + 1, fp_iColonStart - fp_iQuoteStart -1)
					fp_sTail = Mid(fp_sQry, fp_iEnd + 2, fp_iQuoteEnd - fp_iEnd - 2)
					If fp_sQuoteDelim =  """"  Then
						fp_sLead = Replace(fp_sLead, """""", """")
						fp_sTail = Replace(fp_sTail, """""", """")
					ElseIf fp_sQuoteDelim = "'" Then
						fp_sLead = Replace(fp_sLead, "''", "'")
						fp_sTail = Replace(fp_sTail, "''", "'")
					End If
					
					fp_sValue = fp_sLead & fp_sValue & fp_sTail
				End If
		        
				fp_dictParams.Add fp_iParam, fp_sValue
				fp_iParam = fp_iParam + 1
				fp_sValue = "?"	
			else
			' this next finds the named form field value, and substitutes in
			' doubled single-quotes for all single quotes in the literal value
			' so that SQL doesn't get confused by seeing unpaired single-quotes
				Err.Description = "Your page contains a query with user input parameters that could not be resolved.<br>" 
				Err.Description = Err.Description & "This could happen if your DatabaseRegionStart webbot has an empty or missing s-columnnames or s-columntypes attributes.<br>You may need to read Microsoft Knowledge Base Article 817029.<br>"
				fp_fError = True
			End If
			
			If((Len(fp_sQry) - fp_iQuoteEnd) < 1) then
				fp_sQry = Left(fp_sQry, fp_iQuoteStart - 1) & "?"
			else
				fp_sQry = Left(fp_sQry, fp_iQuoteStart - 1) & "?" & Right(fp_sQry, Len(fp_sQry) - fp_iQuoteEnd)
			End If
		Else
			fp_fError=True
			Err.Description = "Unable to find operator in query string.  Query string currently is " & fp_sQry
		End If
		
		' Fixup the new current position to be after the substituted value
		fp_iCurrent = fp_iQuoteStart + 1
	End If
	End If 
	fp_bSkip = false
Loop

' establish connection
If Not fp_fError Then
	if Application(fp_sDataConn & "_ConnectionString") = "" then
		if fp_DEBUG Then
			Err.Description = "The database connection named '" & fp_sDataConn & "' is undefined.<br><br>This problem can occur if:<br>* the connection has been removed from the web<br>* the file 'global.asa' is missing or contains errors<br>* the root folder does not have Scripting permissions enabled<br>* the web is not marked as an Application Root<br>"
		else 
			Err.Description = "The operation failed.  If this continues, please contact your server administrator.<br>"
		end if	
		fp_fError = True
	end if
	if Not fp_fError then
		set fp_conn = Server.CreateObject("ADODB.Connection")
		fp_conn.ConnectionTimeout = Application(fp_sDataConn & "_ConnectionTimeout")
		fp_conn.CommandTimeout = Application(fp_sDataConn & "_CommandTimeout")
		fp_sConn = Application(fp_sDataConn & "_ConnectionString")
		fp_sUid = Application(fp_sDataConn & "_RuntimeUserName")
		fp_sPwd = Application(fp_sDataConn & "_RuntimePassword")
		Err.Clear
		FP_OpenConnection fp_conn, fp_sConn, fp_sUid, fp_sPwd, Not(fp_fCustomQuery)
		if Err.Description <> "" then fp_fError = True
	end if
	if Not fp_fError then
		set fp_cmd = Server.CreateObject("ADODB.Command")
		fp_cmd.CommandText = fp_sQry
		fp_cmd.CommandType = fp_iCommandType
		set fp_cmd.ActiveConnection = fp_conn
		set fp_rs = Server.CreateObject("ADODB.Recordset")
		set fp_rs.Source = fp_cmd
		
		On Error Resume Next
		fp_iTemp = 1
		Do While fp_iTemp < fp_iParam
			fp_colType = fp_dictColTypes.Item(fp_iTemp)
			fp_colValue = fp_dictParams.Item(fp_iTemp)
			
			select case fp_colType 
				case 129, 200, 201, 130, 202, 203 ' adChar, adVarChar, adLongVarChar, adWChar, adVarWChar, adLongVarWChar
					fp_cmd.Parameters.Append = fp_cmd.CreateParameter("Field"&fp_iTemp, fp_colType, 1, Len(fp_colValue) + 1)		
				case else
					fp_cmd.Parameters.Append = fp_cmd.CreateParameter("Field"&fp_iTemp, fp_colType, 1 )		
			end select
			fp_cmd.Parameters("Field"&fp_iTemp).Value = fp_colValue
			fp_iTemp = fp_iTemp + 1
		LOOP
		On Error Goto 0
		
		If fp_iCommandType = 4 Then
			fp_cmd.Parameters.Refresh
			Do Until Len(fp_sInputs) = 0
				fp_iLoc = InStr(fp_sInputs,"=")
				if fp_iLoc = 0 then exit do
				fp_sKey = Left(fp_sInputs,fp_iLoc - 1)
				fp_sInputs = Mid(fp_sInputs,fp_iLoc + 1)
				fp_iLoc = InStr(fp_sInputs,"&")
				if fp_iLoc = 0 then
					fp_sInpVal = fp_sInputs
					fp_sInputs = ""
				else
					fp_sInpVal = Left(fp_sInputs,fp_iLoc - 1)
					fp_sInputs = Mid(fp_sInputs,fp_iLoc + 1)
				end if			
				fp_sVal = Request.Form(fp_sKey)
				if len(fp_sVal) = 0 then fp_sVal = Request.QueryString(fp_sKey)
				if len(fp_sVal) = 0 then fp_sVal = fp_sInpVal
				fp_pType = fp_cmd.Parameters(fp_sKey).Type
				select case fp_pType
					case 129, 200, 201, 130, 202, 203 ' adChar, adVarChar, adLongVarChar, adWChar, adVarWChar, adLongVarWChar
						fp_cmd.Parameters(fp_sKey).Size = Len(fp_sVal) + 1
					case else
						' do nothing
				end select

				' remember names and values used in query
				if not fp_dictInputs.Exists(fp_sKey) then
					fp_dictInputs.Add fp_sKey, fp_sVal
				end if

				fp_cmd.Parameters(fp_sKey) = fp_sVal
			Loop
		End If
		If fp_iMaxRecords <> 0 Then fp_rs.MaxRecords = fp_iMaxRecords

		FP_SetCursorProperties(fp_rs)

		FP_OpenRecordset(fp_rs)
	end if

    if(Err.Description = ""	) then
		' Check for the no-record case
		if fp_rs.State <> 1 then
			fp_fError = True
			Response.Write fp_sNoRecords
		ElseIf fp_rs.EOF And fp_rs.BOF Then
			fp_fError = True
			Response.Write fp_sNoRecords
		end if
    end if
end if

If Err.Description <> "" Then
	if fp_fTableFormat then
		Response.Write "<tr><td colspan=" & fp_iDisplayCols & " color=#000000 bgcolor=#ffff00>"
	end if
	Response.Write "<tt>"
	Response.Write "<b>Database Results Wizard Error</b><br>"
	if fp_DEBUG Then
		if Not fp_fError Then
				Response.Write "<i>Description:</i> " & Server.HtmlEncode(Err.Description) & "<br>"
				Response.Write "<i>Number:</i> " & Server.HtmlEncode(Err.Number) & " (0x" & Hex(Err.Number) & ")<br>"
				Response.Write "<i>Source:</i> " & Server.HtmlEncode(Err.Source) & "<br>"
		else
			Response.Write Err.Description
		end if
		if fp_bBlankField Then
			Response.Write "<br>One or more form fields were empty. You should provide default values for all form fields that are used in the query."
		end if
	else
			Response.Write "The operation failed.  If this continues, please contact your server administrator.<br>"
	end if
	Response.Write "</tt>"
	if fp_fTableFormat then
		Response.Write "</td></tr>"
	end if
	fp_fError = True
end if	



' determine whether or not provider supports Absolute Positioning
if not fp_fError then
	if IsObject(fp_rs) and not(fp_rs.Supports(&H00004000)) then 
		fp_iPageSize = 0
		fp_fShowNavbar = False
	end if
end if

' move to correct position in result set
if not fp_fError then

    if fp_iPageSize > 0 then
		fp_iAbsPage = 1
		fp_sVal = Session(fp_sEnvKey)
		if fp_sVal <> "" then 
			fp_iAbsPage = CInt(fp_sVal)
		end if

		fp_rs.PageSize = fp_iPageSize
		if fp_iAbsPage > fp_rs.PageCount then fp_iAbsPage = fp_rs.PageCount
		fp_rs.AbsolutePage = fp_iAbsPage
		if fp_rs.PageCount = 1 then fp_fShowNavbar = False

		select case fp_sMoveType
			case ""
				' do nothing
			case fp_sFirstLabel
				fp_rs.AbsolutePage = 1
			case fp_sPrevLabel
				if fp_rs.AbsolutePage > 1 then fp_rs.AbsolutePage = fp_rs.AbsolutePage - 1
			case fp_sNextLabel
				if fp_rs.AbsolutePage < fp_rs.PageCount then fp_rs.AbsolutePage = fp_rs.AbsolutePage + 1
			case fp_sLastLabel
				fp_rs.AbsolutePage = fp_rs.PageCount
			case else
				' do nothing
		end select

		fp_iAbsPage = fp_rs.AbsolutePage
		Session(fp_sEnvKey) = fp_iAbsPage
    end if

end if

if fp_fError then fp_fShowNavbar = False

fp_iCount = 0
Do
    if fp_fError then exit do
    if fp_rs.EOF then exit do
    if fp_iPageSize > 0 And fp_iCount >= fp_rs.PageSize then exit do
    if fp_iMaxRecords > 0 And fp_iCount >= fp_iMaxRecords then 
	' MaxRecords didn't work; exit loop
	fp_fShowNavbar = False
	exit do
    end if
%>


