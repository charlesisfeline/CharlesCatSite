

<% Response.Buffer = True %>

<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>Results -- Delete</title>
<meta name="GENERATOR" content="Microsoft FrontPage 6.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
</head>

<body bgcolor="#FFFFFF">

<%    If Request("btnAction") = "New" Then
       Response.Redirect ""
    End If
%>
<%    If Request.QueryString("FP_Num_Items") = "" Then
	fp_sMyQry = ""	
		If Request.Form("Key") <> "" Then
			nCount = 1
			fp_sMyQry = "&Row0=" & Server.UrlEncode(Request.Form("Key"))
        	else
        	nCount = 0
        	For Each Field In Request.Form
            If Request.Form(Field) = "ON" Then
                    fp_sMyQry = fp_sMyQry & "&Row" & nCount & "=" & Field & ""
               
                nCount = nCount + 1
            End If
        Next 'Field
	End If
        If Len(fp_sMyQry) > 0 Then
            sRedirect = "delete.asp?FP_Num_Items=" & nCount & fp_sMyQry
            Response.Redirect sRedirect
        End If
    End If
%>

<% if Len(Request("ConfirmDelete")) = 0 then %>

<!--webbot bot="PurpleText" PREVIEW="-Important-  If you modify this Database Results region using the Database Results Wizard, then your Database Editor will no longer work.  If you accidentally open the Database Results Wizard, simply click Cancel to exit without regenerating the Database Results region." -->
<table border="0">
	<thead>
	<tr>
<td bgcolor="black"><font color="white" size="2"><b>Key</b></font></td>
<td bgcolor="black"><font color="white" size="2"><b>Name</b></font></td>
<td bgcolor="black"><font color="white" size="2"><b>Choice</b></font></td>

	</tr>
	<thead>

	<tbody>
	
<!--#include file="../../../_fpclass/fpdblib.inc"-->
<% if 0 then %>
<SCRIPT Language="JavaScript">
document.write("<div style='background: yellow; color: black;'>The Database Results component on this page is unable to display database content. The page must have a filename ending in '.asp', and the web must be hosted on a server that supports Active Server Pages.</div>");
</SCRIPT>
<% end if %>
<%
fp_sQry="SELECT * FROM Results WHERE Key = ::Row0:: "
fp_iItems = CInt(Request("FP_Num_Items"))
fp_iTemp = 1
do while(fp_iTemp < fp_iItems) 
    fp_sQry = fp_sQry & " OR Key = ::Row" & fp_iTemp & "::"
    fp_iTemp = fp_iTemp + 1
loop
fp_sNoRecords="<tr><td colspan=4 align=""LEFT"" width=""100%"">No records returned.</td></tr>"
fp_sDataConn="database1"
fp_iMaxRecords=256
fp_iCommandType=1
fp_fTableFormat=True
fp_fMenuFormat=False
fp_sColTypes="&Key=3&Name=202&Choice=2"
fp_iDisplayCols=4
fp_fCustomQuery=False
BOTID=0
fp_iRegion=BOTID
%>



<!--#include file="../../../_fpclass/fpdbrgn1.inc"-->

	<tr>
<td>
<!--webbot bot="DatabaseResultColumn" s-columnnames="Key,Name,Address,Choice" s-column="Key" b-tableformat="TRUE" b-hasHTML="FALSE" clientside startspan b-makelink b-MenuFormat preview="&lt;font size=&quot;-1&quot;&gt;&amp;lt;&amp;lt;&lt;/font&gt;Key&lt;font size=&quot;-1&quot;&gt;&amp;gt;&amp;gt;&lt;/font&gt;" --><%=FP_FieldVal(fp_rs,"Key")%><!--webbot bot="DatabaseResultColumn" endspan --></td>
<td>
<!--webbot bot="DatabaseResultColumn" s-columnnames="Key,Name,Address,Choice" s-column="Name" b-tableformat="TRUE" b-hasHTML="FALSE" clientside startspan b-makelink b-MenuFormat preview="&lt;font size=&quot;-1&quot;&gt;&amp;lt;&amp;lt;&lt;/font&gt;Name&lt;font size=&quot;-1&quot;&gt;&amp;gt;&amp;gt;&lt;/font&gt;" --><%=FP_FieldVal(fp_rs,"Name")%><!--webbot bot="DatabaseResultColumn" endspan --></td>
<td>
<!--webbot bot="DatabaseResultColumn" s-columnnames="Key,Name,Address,Choice" s-column="Choice" b-tableformat="TRUE" b-hasHTML="FALSE" clientside startspan b-makelink b-MenuFormat preview="&lt;font size=&quot;-1&quot;&gt;&amp;lt;&amp;lt;&lt;/font&gt;Choice&lt;font size=&quot;-1&quot;&gt;&amp;gt;&amp;gt;&lt;/font&gt;" --><%=FP_FieldVal(fp_rs,"Choice")%><!--webbot bot="DatabaseResultColumn" endspan --></td>

	</tr>
	
<!--#include file="../../../_fpclass/fpdbrgn2.inc"-->
	</tbody>
</table>

<center>
<form method="POST">	
        <%select case fp_iCount %>
       	<%case 0:%>
            <p><b><font size="4">Click OK to refresh.</font></b></p>
            <p><input type="submit" value="OK" name="ConfirmDelete"></p>
        <%case 1:%>
            <p><b><font size="4">Are you sure you want to delete this record?</font></b></p>
            <p><input type="submit" value="     OK     " name="ConfirmDelete"> <input type="submit" value="Cancel" name="ConfirmDelete"></p>
        <%case else:%>
            <p><b><font size="4">Are you sure you want to delete these records?</font></b>
            <p><input type="submit" value="     OK     " name="ConfirmDelete"> <input type="submit" value="Cancel" name="ConfirmDelete"></p>
        <%end select%></form>

</center>

<%
        Response.end
    ElseIf Request("ConfirmDelete") = "Cancel" Then
        Response.Clear
        Response.Redirect "detail.asp"
    End If
%>

<script Language="JavaScript">
<!--
	top.list.location.href = top.list.location.href;
// -->
</script>





<!--#include file="../../../_fpclass/fpdblib.inc"-->
<% if 0 then %>
<SCRIPT Language="JavaScript">
document.write("<div style='background: yellow; color: black;'>The Database Results component on this page is unable to display database content. The page must have a filename ending in '.asp', and the web must be hosted on a server that supports Active Server Pages.</div>");
</SCRIPT>
<% end if %>
<%
fp_sQry="DELETE FROM Results WHERE Key = ::Row0:: "
fp_iItems = CInt(Request("FP_Num_Items"))
fp_iTemp = 1
do while(fp_iTemp < fp_iItems) 
    fp_sQry = fp_sQry & " OR Key = ::Row" & fp_iTemp & "::"
    fp_iTemp = fp_iTemp + 1
loop
fp_sDefault=""
fp_sNoRecords="Record deleted from table."
fp_sDataConn="database1"
fp_iMaxRecords=256
fp_iCommandType=1
fp_iPageSize=0
fp_fTableFormat=False
fp_fMenuFormat=False
fp_sMenuChoice=""
fp_sMenuValue=""
fp_sColTypes="&Key=3&Name=202&Choice=2"
fp_fCustomQuery=True
BOTID=1
fp_iRegion=BOTID

%>
<!--#include file="../../../_fpclass/fpdbrgn1.inc"-->
<p><!--webbot bot="PurpleText" PREVIEW="This is the DELETE query." --></p>
<!--#include file="../../../_fpclass/fpdbrgn2.inc"-->
</body>

</html>
