namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class petlevelup : UserControl
    {
        protected Button btnedit;
        protected DropDownList ddpetlist;
        protected RequiredFieldValidator Requiredfieldvalidator6;

        private void btnedit_Click(object sender, EventArgs e)
        {
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int petdbidx = int.Parse(this.ddpetlist.SelectedValue.ToString());
            int num3 = int.Parse(base.Application["game.petlvupgold2"].ToString());
            int num4 = int.Parse(base.Application["game.petlvupgold3"].ToString());
            string str = new WebLogic().petlevelup(base.Session["userid"].ToString(), useridx, petdbidx, num3, num4);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            else if (!this.Page.IsPostBack)
            {
                this.ddpetlist.DataSource = new DataProviders().ExecuteSqlDs("select petdbidx,name+'['+convert(varchar,grade)+'級]' as name from mhcmember..web_itemlist_bin a,mhgame..tb_petinfo b,mhgame..tb_item c where a.id=c.item_idx and b.summonitemdbidx=c.item_dbidx and b.masteridx=" + base.Session["useridx"].ToString(), "ddpetlist");
                this.ddpetlist.DataTextField = "name";
                this.ddpetlist.DataValueField = "petdbidx";
                this.ddpetlist.DataBind();
            }
        }
    }
}
